#ifndef MODULE_COMMAND_CONTROLS_COMMS_H
#define MODULE_COMMAND_CONTROLS_COMMS_H

#include <stcp/stcp.h>
#include <sppclient/api.h>
#include <sppclient/defs.h>

#include "spp_property_list.h"

void CommandControlComms_Init();

void CommandControlsComms_Task(void* task_args);


#endif
