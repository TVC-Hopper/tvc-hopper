#include "modules/lqr_control.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/hw.h"
#include "hw/thrust_vanes.h"
#include "hw/esc.h"
#include "modules/control_inputs.h"
#include "circbuf/cbuf.h"

SemaphoreHandle_t controls_start_sem;
SemaphoreHandle_t stop_flag_mx;
bool stop_flag;

hovctrl_status_t hover_status;
static float ref[STATE_VECTOR_SIZE] = {0};
static float curr_state[STATE_VECTOR_SIZE] = {0};
static float actuator_input_now[ACTUATION_VECTOR_SIZE] = {0};
static float error_zint = 0;
static float vz = 0;
static float z_last = 0;

static const float K_hover[ACTUATION_VECTOR_SIZE * STATE_VECTOR_SIZE] = {    
   70.7107,   -0.0000,    5.0000,    8.5803,   -0.0000,    3.5687,   -0.0000,   -0.0000,   -0.0000,
   -0.0000,  -70.7107,   -5.0000,   -0.0000,   -9.6246,   -3.5687,    0.0000,    0.0000,    0.0000,
   70.7107,    0.0000,   -5.0000,    8.5803,    0.0000,   -3.5687,    0.0000,    0.0000,    0.0000,
   -0.0000,  -70.7107,    5.0000,   -0.0000,   -9.6246,    3.5687,   -0.0000,   -0.0000,   -0.0000,
    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    4.4624,    1.1935,    6.6667
}; // roll,     pitch,      yaw,        gx,         gy,         gz,         z,        vz,       zint

static void HoverControl_SetStatus(float error_z);
static HOVCTRL_MATH_STATUS_T MultiplyMatrix(float* Result, const float* A, const float* B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows);
static HOVCTRL_MATH_STATUS_T ComputeError(float* Result, float* A, float* B, uint32_t A_size, uint32_t B_size);
static void CorrectYaw(float* error);
static void RateLimit_VaneActuation(float* actuator_input_last, float* actuator_input_now, float alpha);
static float RateLimit(float last, float now, float alpha);
static float Limit(float value, float min, float max);
static void AdjustZError(float* error);
static void ComputeZInt(float error_z);
static void ComputeVZ(float z_now);

static void ExecuteControlStep(TickType_t *last_wake_time);
static void ResetControls();

static float actuator_input_last[ACTUATION_VECTOR_SIZE];

static float lidar_zero = 0.0;

extern void HoverControl_Init() {
    stop_flag = true;
    controls_start_sem = xSemaphoreCreateBinary();
    stop_flag_mx = xSemaphoreCreateMutex();

    hover_status = HOVCTRL_STATUS_STATIONARY;
    // reference already initialized to 0, incl roll/pitch/yaw/gyro
}

extern void HoverControl_Start() {
    // if start is called, do not stop immediately
    xSemaphoreTake(stop_flag_mx, 0xFFFF);
    stop_flag = false;
    xSemaphoreGive(stop_flag_mx);

    xSemaphoreGive(controls_start_sem);
}

extern void HoverControl_Stop() {
    xSemaphoreTake(stop_flag_mx, 0xFFFF);
    stop_flag = true;
    xSemaphoreGive(stop_flag_mx);

    // need to take so that we wait again for start flag
    xSemaphoreTake(controls_start_sem, 0);
}

extern void HoverControl_Task(void* task_args) {

    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;) {

        // set stop flag initially so loop waits for start trigger
        // check stop flag first
        // check reset flag next
        xSemaphoreTake(stop_flag_mx, 0xFFFF);
        if (stop_flag) {
            xSemaphoreGive(stop_flag_mx);

            vTaskDelay(100);
            HwEsc_SetOutputControlBatch(1000.0);
            vTaskDelay(100);
            Hw_UpdatePwm(); 

            // try to take, if can't take wait
            while(pdTRUE != xSemaphoreTake(controls_start_sem, 0xFFFF)) {}
            // release immediately after so that Stop can take
            xSemaphoreGive(controls_start_sem);
            ResetControls();
            // get awake time
            ControlsInputs_NotifyStart();
            
            // will be static at this point, just need to fill data before 1st read
            vTaskDelay(5);
            ControlsInputs_GetLidar(&lidar_zero); 
            xLastWakeTime = xTaskGetTickCount();
        }
        xSemaphoreGive(stop_flag_mx);

        ExecuteControlStep(&xLastWakeTime);
    }
}

static void ResetControls() {
    error_zint = 0;
    vz = 0;
    z_last = 0;
    memset(ref, 0, STATE_VECTOR_SIZE * sizeof(float));
    memset(curr_state, 0, STATE_VECTOR_SIZE * sizeof(float));
    HwThrustVane_GetPositions(actuator_input_last);
    actuator_input_last[4] = HwEsc_GetOutput();
}

static void ExecuteControlStep(TickType_t* last_wake_time) {
    ControlsInputs_GetIMU(&curr_state[STATE_IDX_ROLL]); 
    ControlsInputs_GetLidar(&curr_state[STATE_IDX_Z]);
    
    // zero out lidar (before cm conversion!)
    curr_state[STATE_IDX_Z] -= lidar_zero;
    
    // convert lidar measurement from cm to m
    curr_state[STATE_IDX_Z] /= (float)100.0f;

    // start next reading
    ControlsInputs_NotifyStart();

    float error[STATE_VECTOR_SIZE] = {0};
    ComputeError(error, ref, curr_state, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE);
    CorrectYaw(error);

    // determine flying mode based on altitude
    HoverControl_SetStatus(error[STATE_IDX_Z]);

    // get z error and z velocity
    AdjustZError(error);
    ComputeZInt(error[STATE_IDX_Z]);
    error[STATE_IDX_ZINT] = error_zint;
    ComputeVZ(curr_state[STATE_IDX_Z]);
    error[STATE_IDX_VZ] = vz;
    z_last = curr_state[STATE_IDX_Z];
    
    if (HOVCTRL_MATH_STATUS_OK == MultiplyMatrix(actuator_input_now, K_hover, error, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE)) {

        // flip opposite-facing servos
        actuator_input_now[0] *= -1;
        actuator_input_now[3] *= -1;

        // servos are oriented 90 degrees off 0
        for (uint8_t i = 0; i < 4; ++i) {
            actuator_input_now[i] += 90.0;
        }

        // RateLimit_VaneActuation(actuator_input_last, actuator_input_now, 0.3);
        HwThrustVane_SetPositionsControlBatch(actuator_input_now);
        
        //esc output should be between after matrix multiplication 0 and 1
        float esc_output = actuator_input_now[4] / 0.001 + 1000.0;

        if (esc_output > MAX_ESC) {
            esc_output = MAX_ESC;
        } else if (esc_output < 1000.0) {
            esc_output = 1000.0;
        }

        HwEsc_SetOutputControlBatch(esc_output);
        Hw_UpdatePwm(); 
    }
    


    memcpy(actuator_input_last, actuator_input_now, sizeof(actuator_input_last));
    xTaskDelayUntil(last_wake_time, CONTROL_LOOP_INTERVAL * portTICK_PERIOD_MS);
}

extern void HoverControl_SetReference(float* setpoints) {
    // force target position to be 0 or above a threshold takeoff height
    
    if (setpoints[SETPOINT_Z] <= 0) {
        ref[STATE_IDX_Z] = 0;
    }
    else {
        ref[STATE_IDX_Z] = Limit(setpoints[SETPOINT_Z], SETPOINT_MIN_Z_NONZERO, SETPOINT_MAX_Z);
    }
}

extern void HoverControl_GetState(float* tlm) {
    for (STATE_IDX_T i = 0; i < 6; ++i) {
        tlm[STATE_IDX_ROLL] = curr_state[STATE_IDX_ROLL];
    }
}

extern void HoverControl_GetThrottlePercent(float* throttle) {
    *throttle = actuator_input_now[4];
};

extern hovctrl_status_t HoverControl_GetStatus() {
    return hover_status;
}

static void HoverControl_SetStatus(float error_z) {
    if (ref[STATE_IDX_Z] == 0) {
        if (error_z > -0.01) { // 1 cm above ground
            hover_status = HOVCTRL_STATUS_STATIONARY;
        }
        else if (error_z > -1.0 * SETPOINT_MIN_Z_NONZERO) { // 20 cm above ground
            hover_status = HOVCTRL_STATUS_LANDING_CLOSE;
        }
        else {
            hover_status = HOVCTRL_STATUS_LANDING_FAR;
        } 
    }
    else if (curr_state[STATE_IDX_Z] < SETPOINT_MIN_Z_NONZERO) {
        hover_status = HOVCTRL_STATUS_TAKEOFF;
    }
    else {
        hover_status = HOVCTRL_STATUS_FLYING;
    }
}

static HOVCTRL_MATH_STATUS_T MultiplyMatrix(float* Result, const float* A, const float* B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows) {
    if (A_cols != B_rows) {
        return HOVCTRL_MATH_STATUS_ERROR;
    } 

    for (uint32_t r = 0; r < A_rows; ++r) {
        Result[r] = 0;
        for (uint32_t i = 0; i < A_cols; ++i) {
            Result[r] += A[r * A_cols + i] * B[i];
        }
    }

    return HOVCTRL_MATH_STATUS_OK;
}

static HOVCTRL_MATH_STATUS_T ComputeError(float* Result, float* A, float* B, uint32_t A_size, uint32_t B_size) {
    if (A_size != B_size) {
        return HOVCTRL_MATH_STATUS_ERROR;
    }

    for (uint32_t i = 0; i < A_size; ++i) {
        Result[i] = A[i] - B[i];
    }

    return HOVCTRL_MATH_STATUS_OK;
}

static void CorrectYaw(float* error) {
    if (error[STATE_IDX_YAW] > PI) {
        error[STATE_IDX_YAW] -= 2 * PI;
    }
}

static void RateLimit_VaneActuation(float* last, float* now, float alpha) {
    // apply LPF to actuator inputs
    for (ACTUATOR_IDX_T i = 0; i < 4; ++i) {
        now[i] = RateLimit(last[i], now[i], alpha);
        // (now[i] * alpha) + ((1.0 - alpha) * last[i]);
    }
}

static float RateLimit(float last, float now, float alpha) {
    return (now * alpha) + ((1.0 - alpha) * last);
}

static float Limit(float value, float min, float max){
    float output;

    if( value > max ) output = max;
    else if ( value < min ) output = min;
    else output = value;

    return output;
}

static void AdjustZError(float* error) {
    switch (hover_status) {
        case HOVCTRL_STATUS_STATIONARY:
            error[STATE_IDX_Z] = 0;
            break;
        case HOVCTRL_STATUS_LANDING_CLOSE:
            error[STATE_IDX_Z] *= LANDING_SPEED_FACTOR_CLOSE;
            break;
        case HOVCTRL_STATUS_LANDING_FAR:
            error[STATE_IDX_Z] *= LANDING_SPEED_FACTOR_FAR;
            break; 
        case HOVCTRL_STATUS_TAKEOFF:
            // TODO: increase error for initial esc thrust boost?
            break; 
        default: 
            break;
    }
}

static void ComputeZInt(float error_z) {
    error_zint = 0;// += error_z * CONTROL_LOOP_INTERVAL;
}

static void ComputeVZ(float z_now) {
    // TODO: make more robust against unstable LiDAR
    float vz_now = (z_now - z_last) / CONTROL_LOOP_INTERVAL;
    vz = RateLimit(vz_now, vz, 0.5) ; // averages current and last
    // circ buf?
}
