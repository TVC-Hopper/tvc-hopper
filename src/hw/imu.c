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

extern void HwImu_Service() {
    sh2_service();
}

extern void HwImu_Start() {
    Bno085_InitSensorHub(&imu);

    Bno085_EnableReport(&imu, SH2_LINEAR_ACCELERATION, 5000);
    Bno085_EnableReport(&imu, SH2_GYROSCOPE_CALIBRATED, 5000);
    Bno085_EnableReport(&imu, SH2_ROTATION_VECTOR, 5000);
}

extern void HwImu_GetReadings(float* readings) {
    sh2_SensorValue_t lin_a;
    sh2_SensorValue_t gyro;
    sh2_SensorValue_t rot_v;

    Bno085_GetSensorEvent(&imu, SH2_LINEAR_ACCELERATION, &lin_a);
    readings[0] = lin_a.un.linearAcceleration.x; 
    readings[1] = lin_a.un.linearAcceleration.y; 
    readings[2] = lin_a.un.linearAcceleration.z;

    for (int i = 3; i < 10; ++i) {
        readings[i] = 0;
    }
    /*
    Bno085_GetSensorEvent(&imu, SH2_GYROSCOPE_CALIBRATED, &gyro);
    readings[3] = gyro.un.gyroscope.x; 
    readings[4] = gyro.un.gyroscope.y; 
    readings[5] = gyro.un.gyroscope.z; 
    Bno085_GetSensorEvent(&imu, SH2_ROTATION_VECTOR, &rot_v);
    readings[6] = rot_v.un.rotationVector.i; 
    readings[7] = rot_v.un.rotationVector.j; 
    readings[8] = rot_v.un.rotationVector.k; 
    readings[9] = rot_v.un.rotationVector.real; 
    */

}

static uint32_t GetTimestampUs() {
    return 1000 * xTaskGetTickCount();
}

