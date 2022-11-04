#ifndef HW_LIDAR_H
#define HW_LIDAR_H

#include <stdint.h>

extern void HwLidar_Init();

/**
 * Returns distance in cm
 */
extern uint32_t HwLidar_GetDistance();



#endif
