#ifndef HW_LIDAR_H
#define HW_LIDAR_H

#include <stdint.h>

/**
 *  Initialize lidar hardware interface
 */
extern void HwLidar_Init();

/**
 *  Read distance
 *
 *  @return distance in centimeters
 */
extern uint32_t HwLidar_GetDistance();



#endif
