#ifndef HW_IMU_H
#define HW_IMU_H

#include <stdint.h>

/**
 *  Initialize IMU
 */
extern void HwImu_Init();

/**
 *  Write configuration to IMU, enable specific sensor
 *  repoorts, and set up sensor report request intervals.
 */
extern void HwImu_Start();

/**
 *  Process any pending IMU sensor reports.
 *  Must be called periodically to avoid
 *  losing data or reading stale reports.
 *
 *  Should be called at the fastest sensor report
 *  frequency.
 */
extern void HwImu_Service();

/**
 *  Get yaw, pitch, roll, gyro x, gyro y, and gyro z readings.
 *
 *  @param readings buffer to fill with captured sensor readings
 */
extern void HwImu_GetReadings(float* readings);

#endif
