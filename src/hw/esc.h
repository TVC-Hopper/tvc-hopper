#ifndef HW_ESC_H
#define HW_ESC_H

#include <stdint.h>
#include <stdbool.h>

/**
 *  Initialize ESC hardware interface
 */
extern void HwEsc_Init();

/**
 *  Set output pulse width (1000 - 2000 us)
 *
 *  @param pulse_width_us output pulse width (1000-2000us)
 */
extern void HwEsc_SetOutput(float pulse_width_us, bool setldok);

/**
 *  Get the current ESC output
 *
 *  @return current ESC output (1000-2000us)
 */
extern float HwEsc_GetOutput();

/**
 *  Set the maximum output pulse width (1000 - 2000 us)
 *
 *  @param pulse_width_us output pulse width (1000-2000us)
 */
extern void HwEsc_SetMax(float pw);

/**
 *  Set PWM pulse width but wait for PWM synchronization flag ldok
 *  to be set before moving buffered counters to registers.
 *
 *  @param pulse_width_us output pulse width (1000-2000us)
 *
 */
extern void HwEsc_SetOutputControlBatch(float pulse_width_us);

#endif
