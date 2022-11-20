#ifndef MODULE_CONTROLS_INPUTS_H
#define MODULE_CONTROLS_INPUTS_H

#include <stdint.h>

/**
 *  IMU report indices
 */
#define IMU_RAW_IDX_T                   uint16_t
#define IMU_RAW_IDX_LINACCEL            ((uint16_t)0x00)
#define IMU_RAW_IDX_GYRO                ((uint16_t)0x03)
#define IMU_RAW_IDX_QUATI               ((uint16_t)0x06)
#define IMU_RAW_IDX_QUATJ               ((uint16_t)0x07)
#define IMU_RAW_IDX_QUATK               ((uint16_t)0x08)
#define IMU_RAW_IDX_QUATREAL            ((uint16_t)0x09)
#define IMU_RAW_SIZE                    ((uint16_t)0x0A)
// TODO: verify quat indices

#define IMU_PROC_IDX_T                  uint16_t

/**
 *  IMU report indices
 */
#define IMU_PROC_IDX_ROLL               ((uint16_t)0x00)
#define IMU_PROC_IDX_PITCH              ((uint16_t)0x01)
#define IMU_PROC_IDX_YAW                ((uint16_t)0x02)
#define IMU_PROC_IDX_GYRO               ((uint16_t)0x03)
#define IMU_PROC_SIZE                   ((uint16_t)0x06)

/**
 *  Initialize controls inputs task
 */
extern void ControlsInputs_Init();

/**
 *  Controls inputs task
 */
extern void ControlsInputs_Task();

/**
 *  Start new data acquisition
 */
extern void ControlsInputs_NotifyStart();

/**
 *  Get LIDAR measurement
 *
 *  @param distance set with distance read in centimeters
 */
extern void ControlsInputs_GetLidar(float *distance);

/**
 *  Get IMU measurements
 *
 *  @param data buffer to fill with IMU readings
 */
extern void ControlsInputs_GetIMU(float *data);


#endif
