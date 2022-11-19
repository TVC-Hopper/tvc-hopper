#ifndef HW_THRUST_VANES_H
#define HW_THRUST_VANES_H

#include <stdint.h>

/**
 *  Initialize thrust vane hardware interface
 */
extern void HwThrustVane_Init();

/**
 *  Set servo positions (0-180 degrees)
 *
 *  @param array of 4 floats with angles specified in degrees
 */
extern void HwThrustVane_SetPositions(float* positions);

/**
 *  Get servo positions (0-180 degrees)
 *
 *  @param array of 4 floats to fill with angles specified in degrees
 */
extern void HwThrustVane_GetPositions(float* positions);

/**
 *  Set single thrust vane position
 *
 *  @param idx servo index
 *  @param position servo angle
 */
extern void HwThrustVane_SetPosition(uint8_t idx, float position);

/**
 *  Get single thrust vane position
 *
 *  @param idx servo index
 *  @param position servo angle
 */
extern void HwThrustVane_GetPosition(uint8_t idx, float* position);

#endif
