#ifndef HW_ESC_H
#define HW_ESC_H

#include <stdint.h>

/**
 *  Initialize ESC hardware interface
 */
extern void HwEsc_Init();

/**
 *  Set output pulse_width (1000 - 2000 us)
 *
 *  @param pulse_width_us output pulse width
 */
extern void HwEsc_SetOutput(float pulse_width_us);

/**
 *  Get the current ESC output
 *
 *  @return current ESC output
 */
extern float HwEsc_GetOutput();

#endif
