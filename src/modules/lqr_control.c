#include "modules/lqr_control.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/thrust_vanes.h"
#include "hw/esc.h"
#include "modules/controls_inputs.h"

extern void HoverControl_Init() {
    hover_status = HOVCTRL_STATUS_STATIONARY;
    // reference already initialized to 0, incl roll/pitch/yaw/gx/gy/gz
}

extern void HoverControl_Task(void* task_args) {
    uint32_t xLastWakeTime = xTaskGetTickCount();
    for(;;) {        
        ControlsInputs_GetIMU(&curr_state[STATE_IDX_ROLL]); // TODO verify reading 6 floats

        ControlsInputs_GetLidar(&curr_state[STATE_IDX_Z]); 
        curr_state[STATE_IDX_Z] /= 100; // convert cm to m

        ControlsInputs_NotifyStart();

        float error[STATE_VECTOR_SIZE][1] = {0};

        hover_status = HovCtrl_SetStatus(error);
        float error_z = error[STATE_IDX_Z][0];
        switch (hover_status) {
            case HOVCTRL_STATUS_STATIONARY:
                // TODO drive esc to 0
                break;
            case HOVCTRL_STATUS_LANDING_CLOSE:
                error_z *= LANDING_SPEED_FACTOR_CLOSE;
                break;
            case HOVCTRL_STATUS_LANDING_FAR:
                error_z *= LANDING_SPEED_FACTOR_FAR;
                break; 
            case HOVCTRL_STATUS_TAKEOFF:
                // TODO increase error for initial esc thrust boost?
                break; 
            default: 
                break;
        }
        error_zint += error_z * CONTROL_LOOP_INTERVAL;

        Subtract_Vector(error, ref, curr_state, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE);
        error[STATE_IDX_ZINT][0] = error_zint;

        float actuator_input_last[ACTUATION_VECTOR_SIZE];
        HwThrustVane_GetPositions(actuator_input_last);
        actuator_input_last[4] = HwEsc_GetOutput();
        
        if (HOVCTRL_MATH_STATUS_OK == Multiply_Matrix(actuator_input_now, K_hover, error, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE, 1)) {
            // TODO filter actuation?
            HwThrustVane_SetPositions(actuator_input_now);
            HwEsc_SetOutput(actuator_input_now[4]); // FIXME verify units
        }

        xTaskDelayUntil(&xLastWakeTime, CONTROL_LOOP_INTERVAL * portTICK_PERIOD_MS);

        // TODO telemetry
    }
}

HOVCTRL_MATH_STATUS_T Multiply_Matrix(float* Result, float** A, float** B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows, uint32_t B_cols) {
    // Return float* to make result compatible with setters
    if (A_cols != B_rows || B_cols != 1) {
        return HOVCTRL_MATH_STATUS_ERROR;
    } 

    for (uint32_t r = 0; r < A_rows; ++r) {
        Result[r] = 0;
        for (uint32_t i = 0; i < A_cols; ++i) {
            Result[r] += A[r][i] * B[i][0];
        }
    }

    return HOVCTRL_MATH_STATUS_OK;
}

HOVCTRL_MATH_STATUS_T Subtract_Vector(float** Result, float** A, float** B, uint32_t A_size, uint32_t B_size) {
    // Return float** to make result compatible with Matrix_Multiply()
    if (A_size != B_size) {
        return HOVCTRL_MATH_STATUS_ERROR;
    }

    for (uint32_t i = 0; i < A_size; ++i) {
        Result[i][0] = A[i][0] - B[i][0];
    }

    return HOVCTRL_MATH_STATUS_OK;
}

void Correct_Yaw(float* error) {
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

extern void HovCtrl_SetReference(float* setpoints) {
    ref[STATE_IDX_Z] = setpoints[SETPOINT_Z];
}

extern hovctrl_status_t HovCtrl_SetStatus(float** error) {
    float z_error = error[STATE_IDX_Z][0];

    if (ref[STATE_IDX_Z] == 0) {
        if (z_error > -0.01) {
            return HOVCTRL_STATUS_STATIONARY;
        }
        else if (z_error > -0.2) {
            return HOVCTRL_STATUS_LANDING_CLOSE;
        }
        else {
            return HOVCTRL_STATUS_LANDING_FAR;
        } 
    } 
    else if (ref[STATE_IDX_Z] > 0 && curr_state[STATE_IDX_Z] < 0.2) {
        return HOVCTRL_STATUS_TAKEOFF;
    }
}