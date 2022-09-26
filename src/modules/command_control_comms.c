#include "command_control_comms.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stcp/stcp.h>
#include <sppclient/api.h>
#include <sppclient/defs.h>

#include "spp_property_list.h"

extern SPP_STATUS_T XCCb_StcpSendPacket(void* buffer, uint16_t len, void* instance_data);

static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data);
static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data);
static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data);
static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data);

SppClientEngine_t spp;
StcpEngine_t stcp;

static uint8_t broadcast_addr_raw = 0x00;
static uint8_t addr_raw = 0x01;
static uint8_t property_list_id = 0x00;
static uint8_t client_address_buffer[3];

static SppAddress_t broadcast_addr = { &broadcast_addr_raw };
static SppAddress_t address = { &addr_raw };

extern StcpEngine_t* GetStcpEngine() {
    return &stcp;
}


extern void CommandControlComms_Init() {
    SppClientEngineInitParams_t spp_ip;
    spp_ip.broadcast_address = broadcast_addr;
    spp_ip.client_address = address;
    spp_ip.host_address_buffer = &client_address_buffer;
    spp_ip.property_count = SPP_PROP_COUNT;
    spp_ip.property_list_id = property_list_id;
    spp_ip.address_length = sizeof(uint8_t);
    spp_ip.properties = SppGetPropertyList();

    SppClientCallbacks_t spp_callbacks = {
        .Send = SppSendPacket,
        .SetValue = SetValue,
        .GetValue = GetValue,
    };

    spp_ip.callbacks = spp_callbacks;
    spp_ip.instance_data = NULL;

    SppClientEngineInit(&spp_ip, &spp);

    StcpCallbacks_t stcp_callbacks = {
        .Send = XCCb_StcpSendPacket,
        .HandleMessage = HandleStcpPacket,
    };


    stcp.callbacks = stcp_callbacks;
    stcp.instance_data = NULL;

    SppClientStart(&spp);
}


extern void CommandControlComms_Task(void* task_args) {
    for(;;) {


    }
}

static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case PROP_start_ID:
        {
        }
        case PROP_stop_ID:
        {
        }
        case PROP_status_ID:
        {
        }
        case PROP_telemetry_ID:
        {
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }
}

static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case PROP_start_ID:
        {
        }
        case PROP_stop_ID:
        {
        }
        case PROP_status_ID:
        {
        }
        case PROP_telemetry_ID:
        {
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }
}

static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data) {
    SppClientProcessMessage(&spp, buffer, length);

    return STCP_STATUS_SUCCESS;
}

static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data) {
   StcpWrite(&stcp, bytes, len);

   return SPP_STATUS_OK;
}

