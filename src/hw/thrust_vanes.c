#include "thrust_vanes.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bsp/peripherals.h>

// PWM configuration
#define VANE_PWM_MODE       kPWM_EdgeAligned
#define VANE_PWM            PWM1_PERIPHERAL
#define VANE_0_SM           PWM1_SM2
#define VANE_1_SM           PWM1_SM2
#define VANE_2_SM           PWM1_SM3
#define VANE_3_SM           PWM1_SM1

#define VANE_0_CH           PWM1_SM2_A
#define VANE_1_CH           PWM1_SM2_B
#define VANE_2_CH           PWM1_SM3_A
#define VANE_3_CH           PWM1_SM1_B

// scaling factors
#define MAX_PULSE_WIDTH     ((float) 2.0)
#define MIN_PULSE_WIDTH     ((float) 1.0)
#define MAX_ANGLE           ((float) 180.0)

static void SetPosition(uint8_t idx, float position, bool setldok);

// current positions
static float servo_positions[4] = {90.0, 90.0, 90.0, 90.0};
static float servo_offsets[4] = {0.000, -0.5, 12.600, -3.600};

// indexed by servo id
static const pwm_submodule_t vanes[4] = {
    VANE_3_SM,
    VANE_0_SM,
    VANE_1_SM,
    VANE_2_SM,
};

// indexed by servo id
static const pwm_channels_t vane_channels[4] = {
    VANE_3_CH,
    VANE_0_CH,
    VANE_1_CH,
    VANE_2_CH,
};

// indexed by servo id
static const pwm_module_control_t ctl_modules[4] = {
    kPWM_Control_Module_1,
    kPWM_Control_Module_2,
    kPWM_Control_Module_2,
    kPWM_Control_Module_3,
};

static const pwm_module_control_t all_ctl_modules = ctl_modules[0]
                                                    | ctl_modules[1]
                                                    | ctl_modules[2]
                                                    | ctl_modules[3];

extern void HwThrustVane_Init() {
        // set fault map for each PWM output
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

    // set initial positions
    HwThrustVane_SetPositions(servo_positions);
}

extern void HwThrustVane_SetPositionsControlBatch(float* positions) {
    for (uint8_t i = 0; i < 4; ++i) {
        SetPosition(i, positions[i], false);
    }
}

extern void HwThrustVane_SetPositions(float* positions) {
    for (uint8_t i = 0; i < 4; ++i) {
        SetPosition(i, positions[i], false);
    }

    // synchronize change
    PWM_SetPwmLdok(VANE_PWM, all_ctl_modules, true);
}


extern void HwThrustVane_GetPositions(float* positions) {
    memcpy(positions, servo_positions, sizeof(servo_positions));
}


static void SetPosition(uint8_t idx, float position, bool setldok) {
    servo_positions[idx] = position;

    position += servo_offsets[idx];


    // convert angle to pulse width
    float pulse_width = position / MAX_ANGLE * (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) + MIN_PULSE_WIDTH;

    // caluclate DC from pulse width
    // 50hz -> 20ms period
    uint32_t duty_cycle = (pulse_width / 5.0) * 0xFFFF;

    PWM_UpdatePwmDutycycleHighAccuracy(
            VANE_PWM,
            vanes[idx],
            vane_channels[idx],
            VANE_PWM_MODE,
            duty_cycle
    );

    // only update ldok if not batching sets
    if (setldok) {
        PWM_SetPwmLdok(VANE_PWM, ctl_modules[idx], true);
    }
}

extern void HwThrustVane_SetPosition(uint8_t idx, float position) {
    SetPosition(idx, position, true);
}

extern void HwThrustVane_GetPosition(uint8_t idx, float* position) {
    *position = servo_positions[idx];
}


