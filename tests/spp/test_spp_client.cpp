
#include <catch2/catch_test_macros.hpp>

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <iostream>

#include <sppclient/api.h>
#include <spphost/api.h>

#define SPP_PROP_1_ID ((uint16_t)0xAB)
#define SPP_PROP_2_ID ((uint16_t)0xAC)
#define SPP_PROP_3_ID ((uint16_t)0xAD)

static uint16_t prop_1_value = 10;
static float prop_2_value = 11.1;
static uint16_t prop_3_value[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

static SppPropertyDefinition_t plist[3] = {
    {
        .id = SPP_PROP_1_ID,
        .type = SPP_PROP_T_U16,
        .size = sizeof(uint16_t),
        .flags = SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE,
        .name = "Prop_1\0",
        .name_length = 7,
    },
    {
        .id = SPP_PROP_2_ID,
        .type = SPP_PROP_T_FLOAT,
        .size = sizeof(float),
        .flags = SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE,
        .name = "Prop_2\0",
        .name_length = 7,
    },
    {
        .id = SPP_PROP_3_ID,
        .type = SPP_PROP_T_ARR,
        .size = 10 * sizeof(uint16_t),
        .flags = SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE,
        .name = "Prop_3\0",
        .name_length = 7,
    },
};


static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data) {
    switch (id) {
        case SPP_PROP_1_ID:
        {
            memcpy(&prop_1_value, value, sizeof(prop_1_value));
            break;
        }
        case SPP_PROP_2_ID:
        {
            memcpy(&prop_2_value, value, sizeof(prop_2_value));
            break;
        }
        case SPP_PROP_3_ID:
        {
            memcpy(&prop_3_value, value, sizeof(prop_3_value));
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
        case SPP_PROP_1_ID:
        {
            memcpy(value, &prop_1_value, sizeof(prop_1_value));
            break;
        }
        case SPP_PROP_2_ID:
        {
            memcpy(value, &prop_2_value, sizeof(prop_2_value));
            break;
        }
        case SPP_PROP_3_ID:
        {
            memcpy(value, &prop_3_value, sizeof(prop_3_value));
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }
    return SPP_STATUS_OK;
}

const static int SPPHE_DEFS_BUFFER_SIZE = 3;
static SppPropertyDefinition_t spphe_defs_buffer[SPPHE_DEFS_BUFFER_SIZE];
static uint8_t address_buffer[SPP_CONNECTIONS_MAX * 3];

static uint8_t broadcast_address = 0x00;
static uint8_t client_address = 0x01;
static uint8_t host_address = 0x01;
static uint8_t host_address_buffer;
const uint8_t property_list_id = 0xDD;

static SppAddress_t host = { &host_address };
static SppAddress_t broadcast = { &broadcast_address };
static SppAddress_t client = { &client_address };

static SppClientEngine_t sppce;
static SppHostEngine_t spphe;

static SPP_STATUS_T HostSend(uint8_t* bytes, uint16_t len, void* instance_data) {
    REQUIRE(bytes[0] == SPP_MSG_SYNC_WORD);
    std::cout << "client got id: " << (uint32_t)(bytes[1]) << std::endl;
    SPP_STATUS_T status = SppClientProcessMessage(&sppce, bytes, len);
    for (uint8_t i = 0; i < len; ++i) {
        std::cout << bytes[i];
    }
    std::cout << std::endl;
    if (status) {
        std::cout << "client process: " << (uint32_t)status << std::endl;
    }
    return status;
}

static SPP_STATUS_T ClientSend(uint8_t* bytes, uint16_t len, void* instance_data) {
    REQUIRE(bytes[0] == SPP_MSG_SYNC_WORD);
    std::cout << "host got id: " << (uint32_t)(bytes[1]) << std::endl;
    SPP_STATUS_T status = SppHostProcessMessage(&spphe, bytes, len);
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
    std::cout << "got prop " << id << std::endl;

    switch(id) {
        case SPP_PROP_1_ID:
        {
            std::cout << "\t" << *((uint16_t*)value) << std::endl;
            break;
        }
        case SPP_PROP_2_ID:
        {
            std::cout << "\t" << *((float*)value) << std::endl;
            break;
        }
        case SPP_PROP_3_ID:
        {
            std::cout << "\t";
            for (int i = 0; i < 10; ++i) {
                std::cout << ((uint16_t*)value)[i] << " ";
            }
            std::cout << std::endl;
            break;
        }
        default:
        {
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
    ip.broadcast_address = broadcast;
    ip.client_address = client;
    ip.host_address_buffer = &host_address_buffer;
    ip.property_count = 3;
    ip.property_list_id = property_list_id;
    ip.address_length = sizeof(uint8_t);
    ip.properties = plist;
    ip.callbacks = {
        .Send = ClientSend,
        .SetValue = SetValue,
        .GetValue = GetValue,
    };
    ip.instance_data = nullptr;

    SppClientEngineInit(&ip, &sppce);

    SppHostEngineInitParams_t hip;
    hip.callbacks = SppHostCallbacks_t{
        AreAddressesEqual,
        HostSend,
        OnValueResponse,
        OnStatusResponse,
        OnStreamResponse,
    };
    hip.address_buffer = address_buffer;
    hip.address_length = sizeof(uint8_t);
    hip.defs_buffer = spphe_defs_buffer;
    hip.defs_buffer_size = SPPHE_DEFS_BUFFER_SIZE;
    hip.host_address = SppAddress_t{&host_address};
    hip.instance_data = nullptr;

    SppHostEngineInit(&hip, &spphe);
}


TEST_CASE( "spp client" ) {
    setup();

    SppClientStart(&sppce);
    REQUIRE(spphe.defs_buffer_used == sppce.property_count);
    REQUIRE(spphe.state == SPP_STATE_READY);

    SppHostGetValue(&spphe, &client, SPP_PROP_1_ID);
    SppHostGetValue(&spphe, &client, SPP_PROP_2_ID);
    SppHostGetValue(&spphe, &client, SPP_PROP_3_ID);

    uint16_t prop_1_value_new = 100;
    SppHostSetValue(&spphe, &client, SPP_PROP_1_ID, &prop_1_value_new);

    REQUIRE(prop_1_value == prop_1_value_new);

}






