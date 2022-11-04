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
    ip.sensor_id = 0;
    ip.buffer_size = 32;
    ip.onWrite = XCCb_I2CWrite;
    ip.onRead = XCCb_I2CRead;
    ip.getTime_us = GetTimestampUs;

    Bno085_Init(&imu, &ip);

    // TODO: configure reports
}

extern void HwImu_GetReadings(float* readings) {
    // TODO: what does Tejal need?
}

static uint32_t GetTimestampUs() {
    return 1000 * xTaskGetTickCount();
}

