#include "thrust_vanes.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <drivers/servo.h>

#include "app_hal_xconnect.h"

static Servo_t servos[4];

static float servo_positions[4] = {90.0, 90.0, 90.0, 90.0};

extern void HwThrustVane_Init() {
    ServoInit_t si = {
        .posStart_deg = 90,
        .pulseWidth0deg_us = 2000,
        .pulseWidth180deg_us = 1000,
    };

    void* callbacks[4] = {
        XCCb_ThrustVane_Servo0_Write,
        XCCb_ThrustVane_Servo1_Write,
        XCCb_ThrustVane_Servo2_Write,
        XCCb_ThrustVane_Servo3_Write,
    };

    for (uint8_t i = 0; i < 4; ++i) {
        si.posStart_deg = servo_positions[i];
        si.onNewDutyCycle = callbacks[i];
        Servo_Init(&servos[i], &si);
    }
}

extern void HwThrustVane_Task() {
    while (1) {
        vTaskSuspend(NULL);
    }
}

extern void HwThrustVane_SetPositions(float* positions) {
    memcpy(servo_positions, positions, sizeof(servo_positions));
    for (uint8_t i = 0; i < 4; ++i) {
        Servo_WritePosDeg(&servos[i], (int)positions[i]);
    }
}


extern void HwThrustVane_GetPositions(float* positions) {
    memcpy(positions, servo_positions, sizeof(servo_positions));
}

extern void HwThrustVane_SetPosition(uint8_t idx, float position) {
    servo_positions[idx] = position;
    Servo_WritePosDeg(&servos[idx], (int)position);
}

extern void HwThrustVane_GetPosition(uint8_t idx, float* position) {
    *position = servo_positions[idx];
}


