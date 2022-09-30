
#include <catch2/catch_test_macros.hpp>

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <iostream>

#include <sppclient/api.h>
#include <spphost/api.h>


#define SPP_PROP_START_ID ((uint16_t)0xAB)
#define SPP_PROP_STOP_ID  ((uint16_t)0xAC)
#define SPP_PROP_TELEM_ID ((uint16_t)0xAD)


static bool start = false;
static bool stop = false;
static uint32_t telem[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

#ifdef __cplusplus
extern "C"
{
#endif

static SppPropertyDefinition_t plist[3] = {
    {
        SPP_PROP_START_ID,
        SPP_PROP_T_BOOL,
        sizeof(stop),
        {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        "start\0",
        6,
    },
    {
        SPP_PROP_STOP_ID,
        SPP_PROP_T_BOOL,
        sizeof(start),
        {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        "stop\0",
        5,
    },
    {
        SPP_PROP_TELEM_ID,
        SPP_PROP_T_ARR,
        10 * sizeof(uint32_t),
        SPP_FLAG_READABLE,
        "t\0",
        2,
    },
};

#ifdef __cplusplus
}
#endif

static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data) {
    switch (id) {
        case SPP_PROP_START_ID:
        {
            memcpy(&start, value, sizeof(start));
            break;
        }
        case SPP_PROP_STOP_ID:
        {
            memcpy(&stop, value, sizeof(stop));
            break;
        }
        case SPP_PROP_TELEM_ID:
        {
            memcpy(&telem, value, sizeof(telem));
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case SPP_PROP_START_ID:
        {
            memcpy(value, &start, sizeof(start));
            break;
        }
        case SPP_PROP_STOP_ID:
        {
            memcpy(value, &stop, sizeof(stop));
            break;
        }
        case SPP_PROP_TELEM_ID:
        {
            memcpy(value, &telem, sizeof(telem));
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }
    return SPP_STATUS_OK;
}


// client
const static int SPPHE_DEFS_BUFFER_SIZE = 3;
static SppPropertyDefinition_t spphe_defs_buffer[SPPHE_DEFS_BUFFER_SIZE];
static uint8_t client_address_buffer[SPP_CONNECTIONS_MAX];

static uint8_t broadcast_address_raw = 0x00;
static uint8_t cnc_address_raw = 0x02;
static uint8_t tvc_addr_raw = 0xA3;
static uint8_t host_address_buffer[2];
static uint8_t property_list_id = 0xFF;

static SppAddress_t cnc_addr = { &cnc_address_raw };
static SppAddress_t broadcast_addr = { &broadcast_address_raw };
static SppAddress_t tvc_addr = { &tvc_addr_raw };

static SppClientEngine_t tvc_client;

// host
static SppHostEngine_t cnc_host;

static SPP_STATUS_T HostSend(uint8_t* bytes, uint16_t len, void* instance_data) {
    REQUIRE(bytes[0] == SPP_MSG_SYNC_WORD);
    std::cout << "tvc got id: " << (uint32_t)(bytes[1]) << std::endl;
    SPP_STATUS_T status = SppClientProcessMessage(&tvc_client, bytes, len);
    for (uint8_t i = 0; i < len; ++i) {
        std::cout << bytes[i];
    }
    std::cout << std::endl;
    if (status) {
        std::cout << "tvc process: " << (uint32_t)status << std::endl;
    }
    return status;
}

static SPP_STATUS_T ClientSend(uint8_t* bytes, uint16_t len, void* instance_data) {
    REQUIRE(bytes[0] == SPP_MSG_SYNC_WORD);
    std::cout << "host got id: " << (uint32_t)(bytes[1]) << std::endl;
    SPP_STATUS_T status = SppHostProcessMessage(&cnc_host, bytes, len);
    for (uint8_t i = 0; i < len; ++i) {
        std::cout << bytes[i];
    }
    std::cout << std::endl;
    if (status) {
        std::cout << "host process: " << (uint32_t)status << std::endl;
    }
    return status;
}

static bool AreAddressesEqual(SppAddress_t* a, SppAddress_t* b, void* instance_data) {
    if (!a || !b) {
        return false;
    }
    if (!a->address || !b->address) {
        return false;
    }
    uint8_t a_u8 = (*((uint8_t*)a->address));
    uint8_t b_u8 = (*((uint8_t*)b->address));

    return *((uint8_t*)a->address) == *((uint8_t*)b->address);
}

static SPP_STATUS_T OnValueResponse(SppAddress_t *client, uint16_t id, void* value, void* instance_data) {
    std::cout << "host got prop " << id << std::endl;

    switch(id) {
        case SPP_PROP_START_ID:
        {
            std::cout << "\t" << *((bool*)value) << std::endl;
            break;
        }
        case SPP_PROP_STOP_ID:
        {
            std::cout << "\t" << *((bool*)value) << std::endl;
            break;
        }
        case SPP_PROP_TELEM_ID:
        {
            std::cout << "\t";
            for (int i = 0; i < 10; ++i) {
                std::cout << ((uint32_t*)value)[i] << " ";
            }
            std::cout << std::endl;
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static void OnStatusResponse(SPP_STATUS_T status, void* instance_data) {
    // STUB
}

static void OnStreamResponse(uint32_t timestamp, SppStream_t* stream, void* instance_data) {
    // STUB
}


static void setup() {
    SppClientEngineInitParams_t ip;
    ip.broadcast_address = broadcast_addr;
    ip.client_address = tvc_addr;
    ip.host_address_buffer = &client_address_buffer;
    ip.property_count = SPPHE_DEFS_BUFFER_SIZE;
    ip.property_list_id = property_list_id;
    ip.address_length = sizeof(uint8_t);
    ip.properties = plist;
    ip.callbacks = {
        .Send = ClientSend,
        .SetValue = SetValue,
        .GetValue = GetValue,
    };
    ip.instance_data = nullptr;

    SppClientEngineInit(&ip, &tvc_client);

    SppHostEngineInitParams_t hip;
    hip.callbacks = SppHostCallbacks_t{
        AreAddressesEqual,
        HostSend,
        OnValueResponse,
        OnStatusResponse,
        OnStreamResponse,
    };
    hip.address_buffer = host_address_buffer;
    hip.address_length = sizeof(uint8_t);
    hip.defs_buffer = spphe_defs_buffer;
    hip.defs_buffer_size = SPPHE_DEFS_BUFFER_SIZE;
    hip.host_address = cnc_addr;
    hip.instance_data = nullptr;

    SppHostEngineInit(&hip, &cnc_host);
}



TEST_CASE( "spp tvc client" ) {
    setup();

    SppClientStart(&tvc_client);
    REQUIRE(cnc_host.defs_buffer_used == tvc_client.property_count);
    REQUIRE(cnc_host.state == SPP_STATE_READY);

    SppHostGetValue(&cnc_host, &tvc_addr, SPP_PROP_START_ID);
    SppHostGetValue(&cnc_host, &tvc_addr, SPP_PROP_STOP_ID);
    SppHostGetValue(&cnc_host, &tvc_addr, SPP_PROP_TELEM_ID);

    bool start_new = true;
    SppHostSetValue(&cnc_host, &tvc_addr, SPP_PROP_START_ID, &start_new);

    REQUIRE(start == start_new);

}



