#ifndef APP_HAL_XCONNECT_H
#define APP_HAL_XCONNECT_H

#include <stdint.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <modules/command_control_comms.h>

extern SPP_STATUS_T XCCb_StcpSendPacket(uint8_t* buffer, uint16_t len, void* instance_data);


#endif
