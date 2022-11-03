#include "lqr_control.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "hw/thrust_vanes.h"
#include "hw/esc.h"
#include "hw/lidar.h"

// Had to change include path for portmacro.h in portable.h and add the portable folder to the .gitignore

extern void HoverControl_Task(void* task_args) {
    uint32_t xLastWakeTime = xTaskGetTickCount();
    for(;;) {        
        // (1) get state
        // get sensor readings
        // estimator (if used) is run in another task

        float ref[STATE_VECTOR_SIZE][1];
        float curr_state[STATE_VECTOR_SIZE][1];

        // (2) get error
        // get refernce
        // error = ref - current state

        float error[STATE_VECTOR_SIZE][1];
        MatrixSubtract(error, ref, curr_state, STATE_VECTOR_SIZE, 1, STATE_VECTOR_SIZE, 1);

        // (3) matrix multiply

        // ControlsInputs_NotifyStart();
        float actuator_input[ACTUATION_VECTOR_SIZE];
        if (MatrixMultiply(actuator_input, K_hover, error, STATE_VECTOR_SIZE, ACTUATION_VECTOR_SIZE, STATE_VECTOR_SIZE, 1) == HOVCTRL_STATUS_ERROR) {
            HwThrustVane_GetPositions(actuator_input);
            actuator_input[4] = HwEsc_GetOutput();
        } // if matrix multiply is not successful, return error status and keep previous actuation values

        // (4) send actuation signals
        // TODO filter actuation?

        HwThrustVane_SetPositions(actuator_input);
        HwEsc_SetOutput(actuator_input[4]);

        xTaskDelayUntil(&xLastWakeTime, 5 * portTICK_PERIOD_MS); // 200Hz
    }
}

HOVCTRL_STATUS_T MatrixMultiply(float** Result, float** A, float** B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows, uint32_t B_cols) {

    if (A_cols != B_rows) {
        return HOVCTRL_STATUS_ERROR;
    } 

    for (uint32_t r = 0; r < A_rows; ++r) {
        for (uint32_t c = 0; c < B_cols; ++c) {
            Result[r][c] = 0;
            for (uint32_t i = 0; i < A_cols; ++i) {
                Result[r][c] += A[r][i] * B[i][c];
            }
        }
    }

    return HOVCTRL_STATUS_OK;
}

HOVCTRL_STATUS_T MatrixSubtract(float** Result, float** A, float** B, uint32_t A_rows, uint32_t A_cols, uint32_t B_rows, uint32_t B_cols) {

    if (A_rows != B_rows || A_cols != B_cols) {
        return HOVCTRL_STATUS_ERROR;
    }

    for (uint32_t r = 0; r < A_rows; ++r) {
        for (uint32_t c = 0; c < A_cols; ++c) {
            Result[r][c] = A[r][c] - B[r][c];
        }
    }

    return HOVCTRL_STATUS_OK;
}