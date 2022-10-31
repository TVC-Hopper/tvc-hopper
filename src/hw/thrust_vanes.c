#include "thrust_vanes.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bsp/peripherals.h>


#define VANE_PWM_MODE       kPWM_SignedCenterAligned
#define VANE_PWM            PWM1_PERIPHERAL
#define VANE_0_SM           PWM1_SM2
#define VANE_1_SM           PWM1_SM2
#define VANE_2_SM           PWM1_SM3
#define VANE_3_SM           PWM1_SM3

#define VANE_0_CH           PWM1_SM2_A
#define VANE_1_CH           PWM1_SM2_B
#define VANE_2_CH           PWM1_SM3_A
#define VANE_3_CH           PWM1_SM3_B

#define MAX_DUTY_CYCLE      ((uint16_t)0xFFFF)
#define MAX_ANGLE           ((float) 180.0)

static float servo_positions[4] = {90.0, 90.0, 90.0, 90.0};

// indexed by servo id
static const pwm_submodule_t vanes[4] = {
    VANE_0_SM,
    VANE_1_SM,
    VANE_2_SM,
    VANE_3_SM,
};

// indexed by servo id
static const pwm_channels_t vane_channels[4] = {
    VANE_0_CH,
    VANE_1_CH,
    VANE_2_CH,
    VANE_3_CH,
};

extern void HwThrustVane_Init() {
    for (uint8_t i = 0; i < 4; ++i) {
        PWM_SetupFaultDisableMap(
                VANE_PWM,
                vanes[i],
                vane_channels[i],
                kPWM_faultchannel_0,
                kPWM_FaultDisable_0
                | kPWM_FaultDisable_1
                | kPWM_FaultDisable_2
                | kPWM_FaultDisable_3
        );
    }

    PWM_SetPwmLdok(VANE_PWM, kPWM_Control_Module_2 | kPWM_Control_Module_3, true);
    PWM_StartTimer(
            VANE_PWM, 
            kPWM_Control_Module_2
            | kPWM_Control_Module_3
    );
    HwThrustVane_SetPositions(servo_positions);
}

extern void HwThrustVane_Task() {
    while (1) {
        vTaskSuspend(NULL);
    }
}

extern void HwThrustVane_SetPositions(float* positions) {
    for (uint8_t i = 0; i < 4; ++i) {
        HwThrustVane_SetPosition(i, positions[i]);
    }
}


extern void HwThrustVane_GetPositions(float* positions) {
    memcpy(positions, servo_positions, sizeof(servo_positions));
}

extern void HwThrustVane_SetPosition(uint8_t idx, float position) {
    servo_positions[idx] = position;
    uint16_t duty_cycle = position * (MAX_DUTY_CYCLE / MAX_ANGLE);
    PWM_UpdatePwmDutycycleHighAccuracy(
            VANE_PWM,
            vanes[idx],
            vane_channels[idx],
            VANE_PWM_MODE,
            duty_cycle
    );
    PWM_SetPwmLdok(VANE_PWM, kPWM_Control_Module_2 | kPWM_Control_Module_3, true);
}

extern void HwThrustVane_GetPosition(uint8_t idx, float* position) {
    *position = servo_positions[idx];
}


