#include "modules/lqr_control.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/thrust_vanes.h"
#include "hw/esc.h"
#include "modules/controls_inputs.h"

extern void HoverControl_Init() {
    control_status_t hover_status = CONTROL_STATUS_STATIONARY;

    // TODO set reference
}

extern void HoverControl_Task(void* task_args) {
    uint32_t xLastWakeTime = xTaskGetTickCount();
    for(;;) {        
        float ref[STATE_VECTOR_SIZE] = {0};
        float curr_state[STATE_VECTOR_SIZE] = {0};
        float error[STATE_VECTOR_SIZE][1] = {0};

        // TODO get last base station command to set reference

        ControlsInputs_GetIMU(&curr_state[STATE_IDX_GX]); // FIXME 

        ControlsInputs_GetLidar(&curr_state[STATE_IDX_Z]); 
        curr_state[STATE_IDX_Z] /= 100; // FIXME convert cm to m in getter instead

        Vector_Subtract(error, ref, curr_state, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE);

        ControlsInputs_NotifyStart();

        float actuator_input[ACTUATION_VECTOR_SIZE];
        /*
        if (HOVCTRL_STATUS_ERROR == Matrix_Multiply(actuator_input, K_hover, error, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE, 1)) {
            HwThrustVane_GetPositions(actuator_input);
            actuator_input[4] = HwEsc_GetOutput();
        } // if matrix multiply is not successful, keep previous actuation values
        */
        if (HOVCTRL_STATUS_OK == Matrix_Multiply(actuator_input, K_hover, error, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, STATE_VECTOR_SIZE, 1)) {
            // TODO filter actuation?
            HwThrustVane_SetPositions(actuator_input);
            HwEsc_SetOutput(actuator_input[4]); // FIXME verify units
        }

        xTaskDelayUntil(&xLastWakeTime, CONTROL_LOOP_INTERVAL * portTICK_PERIOD_MS);

        // TODO telemetry
    }
}

HOVCTRL_STATUS_T Matrix_Multiply(float* Result, float** A, float** B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows, uint32_t B_cols) {
    // Return float* to make result compatible with setters
    if (A_cols != B_rows || B_cols != 1) {
        return HOVCTRL_STATUS_ERROR;
    } 

    for (uint32_t r = 0; r < A_rows; ++r) {
        Result[r] = 0;
        for (uint32_t i = 0; i < A_cols; ++i) {
            Result[r] += A[r][i] * B[i][0];
        }
    }

    return HOVCTRL_STATUS_OK;
}

HOVCTRL_STATUS_T Vector_Subtract(float** Result, float** A, float** B, uint32_t A_size, uint32_t B_size) {
    // Return float** to make result compatible with Matrix_Multiply()
    if (A_size != B_size) {
        return HOVCTRL_STATUS_ERROR;
    }

    for (uint32_t i = 0; i < A_size; ++i) {
        Result[i][0] = A[i][0] - B[i][0];
    }

    return HOVCTRL_STATUS_OK;
}

void SetReference() {
    // TODO command module should set 
}