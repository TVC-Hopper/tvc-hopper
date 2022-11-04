#ifndef MODULE_CONTROLS_INPUTS_H
#define MODULE_CONTROLS_INPUTS_H

#include <stdint.h>

extern void ControlsInputs_Init();

extern void ControlsInputs_Task();

extern void ControlsInputs_NotifyStart();

extern void ControlsInputs_GetLidar(uint32_t *distance);

extern void ControlsInputs_GetIMU(float *data);


#endif
