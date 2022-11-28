#include "imu.h"

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <drivers/bno085_i2c.h>

#include <sh2/shtp.h>
#include <sh2/sh2.h>
#include <sh2/sh2_SensorValue.h>
#include <sh2/sh2_err.h>

#include "app_hal_xconnect.h"

static Bno085_t imu;

static uint32_t GetTimestampUs();

extern void HwImu_Init() {
    Bno085InitParams_t ip;
    ip.buffer_size = 32;
    ip.onWrite = XCCb_I2CWrite;
    ip.onRead = XCCb_I2CRead;
    ip.getTime_us = GetTimestampUs;
    ip.onDelay = XCCb_DelayMs;

    Bno085_Init(&imu, &ip);

}

extern void HwImu_Start() {
    Bno085_InitSensorHub(&imu);

    // Bno085_EnableReport(&imu, SH2_LINEAR_ACCELERATION, 5000);
    Bno085_EnableReport(&imu, SH2_GYROSCOPE_CALIBRATED, 5000);
    Bno085_EnableReport(&imu, SH2_ROTATION_VECTOR, 5000);
}

extern void HwImu_GetReadings(float* readings) {
    Bno085_GetSensorEvents(&imu);

    // Bno085_GetSensorValueFloat(&imu, SH2_LINEAR_ACCELERATION, 3, readings);
    Bno085_GetSensorValueFloat(&imu, SH2_GYROSCOPE_CALIBRATED, 3, readings + 3);
    Bno085_GetSensorValueFloat(&imu, SH2_ROTATION_VECTOR, 4, readings + 6);
}

static uint32_t GetTimestampUs() {
    return 1000 * xTaskGetTickCount();
}

