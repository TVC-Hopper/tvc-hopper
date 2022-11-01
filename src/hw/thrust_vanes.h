#ifndef HW_THRUST_VANES_H
#define HW_THRUST_VANES_H

#include <stdint.h>

extern void HwThrustVane_Init();

/**
 * Provide 4 float array of positions
 */
extern void HwThrustVane_SetPositions(float* positions);
extern void HwThrustVane_GetPositions(float* positions);

/**
 * idx is 0-3 for vanes 0, 1, 2, 3
 */
extern void HwThrustVane_SetPosition(uint8_t idx, float position);
extern void HwThrustVane_GetPosition(uint8_t idx, float* position);

#endif
