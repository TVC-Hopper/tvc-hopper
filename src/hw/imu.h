#ifndef HW_IMU_H
#define HW_IMU_H

#include <stdint.h>

extern void HwImu_Init();
extern void HwImu_GyroTask();
extern void HwImu_AccTask();

extern void HwImu_GetAccValues(float *x, float *y, float *z);
extern void HwImu_GetGyroValues(float *x, float *y, float *z);
extern void HwImu_GetTemperature(float *temp);

extern void HwImu_ReleaseGyroTaskFromISR();
extern void HwImu_ReleaseAccTaskFromISR();





#endif
