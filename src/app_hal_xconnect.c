#include "app_hal_xconnect.h"

#include <stdint.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <modules/command_control_comms.h>


// TODO: connect Xbee/UART to comms


extern SPP_STATUS_T XCCb_StcpSendPacket(uint8_t* buffer, uint16_t len, void* instance_data) {
    // TODO: write to UART/xbee
    
    return SPP_STATUS_OK;
}
