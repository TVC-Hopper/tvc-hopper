#ifndef HW_THRUST_VANES_H
#define HW_THRUST_VANES_H

#include <stdint.h>

/**
 *  Initialize thrust vane hardware interface. Sets
 *  thrust vanes to initial positions
 */
extern void HwThrustVane_Init();

/**
 *  Set servo positions (0-180 degrees) immediately.
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
 *  Set servo positions (0-180 degrees) but do not update PWM
 *  registers from PWM buffer. The ldok bit must be set
 *  after calling this function to move buffered PWM configurations
 *  to registers.
 *
 *  This can be used when synchronizing output with other PWM
 *  outputs.
 *
 *  @param array of 4 floats with angles specified in degrees
 */
extern void HwThrustVane_SetPositionsControlBatch(float* positions);

/**
 *  Set single thrust vane position
 *
 *  @param idx servo index
 *  @param position servo angle (0-180 degrees)
 */
extern void HwThrustVane_SetPosition(uint8_t idx, float position);

/**
 *  Get single thrust vane position
 *
 *  @param idx servo index
 *  @param position servo angle (0-180 degrees)
 */
extern void HwThrustVane_GetPosition(uint8_t idx, float* position);

#endif
