#include "modules/lqr_control.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/thrust_vanes.h"
#include "hw/esc.h"
#include "modules/controls_inputs.h"

enum hovctrl_status_t hover_status = HOVCTRL_STATUS_STATIONARY;
static float ref[STATE_VECTOR_SIZE] = {0};
static float curr_state[STATE_VECTOR_SIZE] = {0};
static float actuator_input_now[ACTUATION_VECTOR_SIZE] = {0};
static float error_zint = 0;

static const float K_hover[ACTUATION_VECTOR_SIZE][STATE_VECTOR_SIZE] = {    
    70.711,   0.000,      5.000,      12.161,     0.000,      5.217,      0.000,    0.000,    0.000,
    0.000,    -70.711,    -5.000,     0.000,      -12.162,    -5.217,     0.000,    0.000,    0.000,
    70.711,   0.000,      -5.000,     12.161,     0.000,      -5.217,     0.000,    0.000,    0.000,
    0.000,    -70.711,    5.000,      0.000,      -12.162,    5.217,      0.000,    0.000,    0.000,
    0.000,    0.000,      0.000,      0.000,      0.000,      0.000,      7.716,    4.140,    7.071 
};
//  roll,     pitch,      yaw,        gx,         gy,         gz,         z,        vz,       zint

static void HoverControl_SetStatus(float error_z);
static HOVCTRL_MATH_STATUS_T MultiplyMatrix(float* Result, float** A, float* B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows);
static HOVCTRL_MATH_STATUS_T ComputeError(float* Result, float* A, float* B, uint32_t A_size, uint32_t B_size);
static void CorrectYaw(float* error);
static void RateLimit_VaneActuation(float* actuator_input_last, float* actuator_input_now, float alpha);
static float Limit(float value, float min, float max);
static void ComputeZint (float error_z);

extern void HoverControl_Init() {
    hover_status = HOVCTRL_STATUS_STATIONARY;
    // reference already initialized to 0, incl roll/pitch/yaw/gx/gy/gz
}

extern void HoverControl_Task(void* task_args) {
    uint32_t xLastWakeTime = xTaskGetTickCount();
    enum hovctrl_status_t hover_status = HOVCTRL_STATUS_STATIONARY;

    for(;;) {                
        ControlsInputs_GetIMUProcessed(&curr_state[STATE_IDX_ROLL]); 
            // TODO: verify reading 6 floats [roll, pitch, yaw, gx, gy, gz]

        ControlsInputs_GetLidar(&curr_state[STATE_IDX_Z]); 
        curr_state[STATE_IDX_Z] /= (float)100; // convert cm to m

        ControlsInputs_NotifyStart();

        float error[STATE_VECTOR_SIZE] = {0};
        
        ComputeError(error, ref, curr_state, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE);

        CorrectYaw(error);
        HoverControl_SetStatus(error[STATE_IDX_Z]);
        ComputeZint(error[STATE_IDX_Z]);
        error[STATE_IDX_ZINT] = error_zint;

        float actuator_input_last[ACTUATION_VECTOR_SIZE];
        HwThrustVane_GetPositions(actuator_input_last);
        actuator_input_last[4] = HwEsc_GetOutput();
        
        if (HOVCTRL_MATH_STATUS_OK == MultiplyMatrix(actuator_input_now, K_hover, error, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE)) {
            // TODO: filter actuation
            HwThrustVane_SetPositions(actuator_input_now);
            HwEsc_SetOutput(actuator_input_now[4] * MOTOR_KRPM_TO_ESC_PERCENT); 
                // FIXME: verify units
        }

        xTaskDelayUntil(&xLastWakeTime, CONTROL_LOOP_INTERVAL * portTICK_PERIOD_MS);
    }
}

extern void HoverControl_SetReference(float* setpoints) {
    ref[STATE_IDX_Z] = setpoints[SETPOINT_Z];
}

extern void HoverControl_GetState(float* tlm) {
    for (STATE_IDX_T i = 0; i < 6; ++i) {
        tlm[STATE_IDX_ROLL] = curr_state[STATE_IDX_ROLL];
    }
}

// extern enum hovctrl_status_t HoverControl_GetStatus() {
//     return hover_status;
// }

void HoverControl_SetStatus(float error_z) {
    if (ref[STATE_IDX_Z] == 0) {
        if (error_z > -0.01) {
            hover_status = HOVCTRL_STATUS_STATIONARY;
        }
        else if (error_z > -0.2) {
            hover_status = HOVCTRL_STATUS_LANDING_CLOSE;
        }
        else {
            hover_status = HOVCTRL_STATUS_LANDING_FAR;
        } 
    }
    else if (curr_state[STATE_IDX_Z] < 0.2) { // and ref > 0
        hover_status = HOVCTRL_STATUS_TAKEOFF;
    }
    else {
        hover_status = HOVCTRL_STATUS_FLYING;
    }
}

HOVCTRL_MATH_STATUS_T MultiplyMatrix(float* Result, float** A, float* B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows) {
    if (A_cols != B_rows) {
        return HOVCTRL_MATH_STATUS_ERROR;
    } 

    for (uint32_t r = 0; r < A_rows; ++r) {
        Result[r] = 0;
        for (uint32_t i = 0; i < A_cols; ++i) {
            Result[r] += A[r][i] * B[i];
        }
    }

    return HOVCTRL_MATH_STATUS_OK;
}

HOVCTRL_MATH_STATUS_T ComputeError(float* Result, float* A, float* B, uint32_t A_size, uint32_t B_size) {
    if (A_size != B_size) {
        return HOVCTRL_MATH_STATUS_ERROR;
    }

    for (uint32_t i = 0; i < A_size; ++i) {
        Result[i] = A[i] - B[i];
    }

    return HOVCTRL_MATH_STATUS_OK;
}

void CorrectYaw(float* error) {
    if (error[STATE_IDX_YAW] > PI) {
        error[STATE_IDX_YAW] -= 2 * PI;
    }
}

void RateLimit_VaneActuation(float* actuator_input_last, float* actuator_input_now, float alpha) {
    // Copied from RateLimit function in MT, may need to apply LPF to actuator inputs
    for (ACTUATOR_IDX_T i = 0; i < 4; ++i) {
        actuator_input_now[i] = (actuator_input_now[i] * alpha) + ((1.0 - alpha) * actuator_input_last[i]);
    }
}

float Limit(float value, float min, float max){
    // Copied from MT
    float output;

    if( value > max ){
        output = max;
    }
    else if ( value < min ){
        output = min;
    }
    else{
        output = value;
    }

    return output;
}

void ComputeZint (float error_z) {
    switch (hover_status) {
        case HOVCTRL_STATUS_STATIONARY:
            // TODO: drive esc to 0
            break;
        case HOVCTRL_STATUS_LANDING_CLOSE:
            error_z *= LANDING_SPEED_FACTOR_CLOSE;
            break;
        case HOVCTRL_STATUS_LANDING_FAR:
            error_z *= LANDING_SPEED_FACTOR_FAR;
            break; 
        case HOVCTRL_STATUS_TAKEOFF:
            // TODO: increase error for initial esc thrust boost?
            break; 
        default: 
            break;
    }
    error_zint += error_z * CONTROL_LOOP_INTERVAL;
}

