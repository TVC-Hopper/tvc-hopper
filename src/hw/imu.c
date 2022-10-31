#include "imu.h"

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <drivers/imu.h>

#include "app_hal_xconnect.h"

#define IMU_SAMPLE_FREQUENCY ((TickType_t) 2)

SemaphoreHandle_t gyro_isr_sem = NULL;
SemaphoreHandle_t acc_isr_sem = NULL;

static IMU_t imu;

extern void HwImu_ReleaseGyroTaskFromISR() {
    // TODO switch to task notifications
    xSemaphoreGiveFromISR(gyro_isr_sem, NULL);
}

extern void HwImu_ReleaseAccTaskFromISR() {
    // TODO switch to task notifications
    xSemaphoreGiveFromISR(acc_isr_sem, NULL);
}

extern void HwImu_GetAccValues(float *x, float *y, float *z) {
    *x = imu.Acc.xComp;
    *y = imu.Acc.yComp;
    *z = imu.Acc.zComp;
}

extern void HwImu_GetGyroValues(float *x, float *y, float *z) {
    *x = imu.Gyro.xComp;
    *y = imu.Gyro.yComp;
    *z = imu.Gyro.zComp;
}

extern void HwImu_GetTemperature(float *temp) {
    *temp = imu.degC;
}

extern void HwImu_Init() {
    gyro_isr_sem = xSemaphoreCreateBinary();
    acc_isr_sem = xSemaphoreCreateBinary();

    xSemaphoreTake(gyro_isr_sem, 0xFFFF);
    xSemaphoreTake(acc_isr_sem, 0xFFFF);

    IMUinit_t imu_init = {
        .sd01 = SDO1_LOW,
        .onDelay_ms = XCCb_DelayMs,
        .onI2Cwrite = XCCb_I2CWrite,
        .onI2Cread = XCCb_I2CRead,
    };

    IntInit_t acc_int_init = {
        .IntPin = INT_1,
        .pinIsInput = 0,
        .PpOd = PUSH_PULL,
        .activeLvl = ACT_LOW,
    };

    AccelInit_t acc_init = {
        .range = RANGE_3G,
        .osr = NONE,
        .odr = HZ_12_5,
        .AccInts = acc_int_init,
    };

    IntInit_t gyro_int_init = {
        .IntPin = INT_3,
        .pinIsInput = 0,
        .PpOd = PUSH_PULL,
        .activeLvl = ACT_LOW,
    };

    GyroInit_t gyro_init = {
        .range = RATE_2000_DS,
        .OdrBw = ODR_100_BW_12,
        .GyroInts = gyro_int_init,
    };

    int ret = IMU_init(&imu, &acc_init, &gyro_init, &imu_init);

    if (!ret) {
        // IMU init failed
        while(1) {}
    }
}

extern void HwImu_GyroTask() {
    while(1) {
        vTaskSuspend(NULL);
        if (xSemaphoreTake(gyro_isr_sem, 0xFFFF) == pdTRUE) {
        }
    }
}

extern void HwImu_AccTask() {
    while(1) {
        vTaskSuspend(NULL);
        if (xSemaphoreTake(acc_isr_sem, 0xFFFF) == pdTRUE) {
        }
    }
}




