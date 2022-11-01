#ifndef HW_ESC_H
#define HW_ESC_H

#include <stdint.h>

extern void HwEsc_Init();

extern void HwEsc_SetOutput(float output);
extern float HwEsc_GetOutput();

#endif
