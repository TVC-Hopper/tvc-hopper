#include "esc.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bsp/peripherals.h>

// PWM configuration
#define ESC_PWM_MODE        kPWM_EdgeAligned
#define ESC_PWM             PWM1_PERIPHERAL
#define ESC_SM              PWM1_SM3
#define ESC_CH              PWM1_SM3_B

#define ESC_PWM_PERIOD      ((float) 20000.0)

#define ESC_MAX             ((float) 2000.0)
#define ESC_MIN             ((float) 1000.0)

// current ESC output
static float esc_output = 1000.0;

static float esc_max = ESC_MAX;
static float esc_min = ESC_MIN;

extern void HwEsc_Init() {
    PWM_SetupFaultDisableMap(
            ESC_PWM,
            ESC_SM,
            ESC_CH,
            kPWM_faultchannel_0,
            kPWM_FaultDisable_0
            | kPWM_FaultDisable_1
            | kPWM_FaultDisable_2
            | kPWM_FaultDisable_3
    );

    HwEsc_SetOutput(esc_output, true);
}

extern void HwEsc_SetOutputControlBatch(float pulse_width_us) {
    HwEsc_SetOutput(pulse_width_us, false);
}

extern void HwEsc_SetMax(float pw) {
    esc_max = pw;
}

extern void HwEsc_SetOutput(float pulse_width_us, bool setldok) {

    if (pulse_width_us > esc_max) {
        pulse_width_us = esc_max;
    } else if (pulse_width_us < esc_min) {
        pulse_width_us = esc_min;
    }

    esc_output = pulse_width_us;

    uint32_t duty_cycle = pulse_width_us / ESC_PWM_PERIOD * 0xFFFF;

    PWM_UpdatePwmDutycycleHighAccuracy(
            ESC_PWM,
            ESC_SM,
            ESC_CH,
            ESC_PWM_MODE,
            duty_cycle
    );

    if (setldok) {
        // set bit to update PWM counters on next cycle (synchronization feature)
        PWM_SetPwmLdok(ESC_PWM, kPWM_Control_Module_3, true);
    }
}


extern float HwEsc_GetOutput() {
    return esc_output;
}


