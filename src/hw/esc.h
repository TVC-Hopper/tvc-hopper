#ifndef HW_ESC_H
#define HW_ESC_H

#include <stdint.h>

/**
 *  Initialize ESC hardware interface
 */
extern void HwEsc_Init();

/**
 *  Set output percentage (0-100%)
 *
 *  @param output percent output
 */
extern void HwEsc_SetOutput(float output);

/**
 *  Get the current ESC output
 *
 *  @return current ESC output
 */
extern float HwEsc_GetOutput();

#endif
