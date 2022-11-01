#include "esc.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bsp/peripherals.h>

#define ESC_PWM_MODE        kPWM_EdgeAligned
#define ESC_PWM             PWM1_PERIPHERAL
#define ESC_SM              PWM1_SM1
#define ESC_CH              PWM1_SM1_B

#define MAX_DUTY_CYCLE ((uint16_t)0xFFFF)
#define MAX_OUTPUT     (100.0f)

static float esc_output = 90.0;

extern void HwEsc_Init() {
    HwEsc_SetOutput(esc_output);
}

extern void HwEsc_Task() {
    while(1) {
        vTaskSuspend(NULL);
    }
}

extern void HwEsc_SetOutput(float output) {
    if (output > MAX_OUTPUT) {
        output = MAX_OUTPUT;
    } else if (output < 0.0) {
        output = 0.0;
    }

    esc_output = output;
    PWM_UpdatePwmDutycycle(
            ESC_PWM,
            ESC_SM,
            ESC_CH,
            ESC_PWM_MODE,
            output
    );
    PWM_SetPwmLdok(ESC_PWM, kPWM_Control_Module_1, true);
}


extern float HwEsc_GetOutput() {
    return esc_output;
}


