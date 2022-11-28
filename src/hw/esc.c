#include "esc.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bsp/peripherals.h>

// PWM configuration
#define ESC_PWM_MODE        kPWM_EdgeAligned
#define ESC_PWM             PWM1_PERIPHERAL
#define ESC_SM              PWM1_SM1
#define ESC_CH              PWM1_SM1_B

#define ESC_PWM_PERIOD      ((float) 20.0f)

// current ESC output
static float esc_output = 90.0;

extern void HwEsc_Init() {
    HwEsc_SetOutput(esc_output);
}

extern void HwEsc_SetOutput(float pulse_width) {
    // TODO: switch to high accuracy and convert PW to DC
    esc_output = output;

    uint32_t duty_cycle = pulse_width / ESC_PWM_PERIOD * 0xFFFF

    PWM_UpdatePwmDutycycleHighAccuracy(
            ESC_PWM,
            ESC_SM,
            ESC_CH,
            ESC_PWM_MODE,
            duty_cycle
    );

    // set bit to update PWM counters on next cycle (synchronization feature)
    PWM_SetPwmLdok(ESC_PWM, kPWM_Control_Module_1, true);
}


extern float HwEsc_GetOutput() {
    return esc_output;
}


