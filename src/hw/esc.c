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
    esc_output = output;
    PWM_UpdatePwmDutycycleHighAccuracy(
            ESC_PWM,
            ESC_SM,
            ESC_CH,
            ESC_PWM_MODE,
            output * (MAX_DUTY_CYCLE / MAX_OUTPUT)
    );
}


extern float HwEsc_GetOutput() {
    return esc_output;
}


