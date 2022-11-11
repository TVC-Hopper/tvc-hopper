#ifndef HW_IMU_H
#define HW_IMU_H

#include <stdint.h>

extern void HwImu_Init();

extern void HwImu_Start();

extern void HwImu_GetReadings(float* readings);

#endif
