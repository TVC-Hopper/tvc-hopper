#include <catch2/catch_test_macros.hpp>

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <iostream>

#include <spphost/api.h>

static SppHostEngine_t spphe;
const static int SPPHE_DEFS_BUFFER_SIZE = 24;
static SppPropertyDefinition_t spphe_defs_buffer[SPPHE_DEFS_BUFFER_SIZE];
static uint8_t address_buffer[SPP_CONNECTIONS_MAX * 3];

static uint8_t host_address = 0x1D;
static uint8_t c0_address = 0x44;
static uint8_t c1_address = 0x45;
static uint8_t c2_address = 0x46;

static SppAddress_t c0 = {&c0_address};
static SppAddress_t c1 = {&c1_address};
static SppAddress_t c2 = {&c2_address};

static uint8_t last_msg[128];
static uint8_t last_msg_len = 0;

static SppStream_t* last_stream;
static void* last_value;
static uint16_t last_id;

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

static SPP_STATUS_T Send(uint8_t* bytes, uint16_t len, void* instance_data) {
    memcpy(last_msg, bytes, len);
    last_msg_len = len;
    return SPP_STATUS_OK;
}


static SPP_STATUS_T OnValueResponse(SppAddress_t *client, uint16_t id, void* value, void* instance_data) {
    std::cout << "got prop " << id << std::endl;
    last_id = id;
    last_value = value;
    return SPP_STATUS_OK;
}

static void OnStatusResponse(SPP_STATUS_T status, void* instance_data) {
    // STUB
}

static void OnStreamResponse(SppStream_t* stream, void* instance_data) {
    last_stream = stream;
    // STUB
}

static void setup() {
    SppHostEngineInitParams_t ip;
    ip.callbacks = SppHostCallbacks_t{
        AreAddressesEqual,
        Send,
        OnValueResponse,
        OnStatusResponse,
        OnStreamResponse,
    };
    ip.address_buffer = address_buffer;
    ip.address_length = sizeof(uint8_t);
    ip.defs_buffer = spphe_defs_buffer;
    ip.defs_buffer_size = SPPHE_DEFS_BUFFER_SIZE;
    ip.host_address = SppAddress_t{&host_address};
    ip.instance_data = nullptr;

    SppHostEngineInit(&ip, &spphe);
}



TEST_CASE( "spp only, no clients" ) {
    setup();

    REQUIRE(spphe.token_count == 0);
    REQUIRE(spphe.defs_buffer_used == 0);
    REQUIRE(spphe.client_count == 0);
    REQUIRE(spphe.pending_request_count == 0);

    REQUIRE(SppHostGetValue(&spphe, &c0, 0) == SPP_STATUS_NO_SUCH_CLIENT);
    REQUIRE(SppHostGetDefinition(&spphe, &c0, 0, nullptr) == SPP_STATUS_NO_SUCH_CLIENT);
    REQUIRE(SppHostSetValue(&spphe, &c0, 0, nullptr) == SPP_STATUS_NO_SUCH_CLIENT);

    SppStream_t s;
    REQUIRE(SppHostStartStream(&spphe, &c0, 0, 1200, SPP_STREAM_READ, &s) == SPP_STATUS_NO_SUCH_CLIENT);
}

TEST_CASE( "message size max" ) {
    REQUIRE(MESSAGE_SIZE(0, sizeof(uint8_t)) == SPP_MSG_HDR_SIZE(sizeof(uint8_t)));
}

TEST_CASE( "spp process received errors" ) {
    setup();
    uint8_t full_len = 4;
    uint8_t short_len = 3;
    uint8_t msg_sync_error[full_len];
    msg_sync_error[0] = 0x1;
    spphe.address_length = sizeof(uint8_t);

    REQUIRE(SppHostProcessMessage(&spphe, msg_sync_error, full_len) == SPP_STATUS_INVALID_SYNC_WORD);

    uint8_t msg_short[short_len];
    msg_short[0] = SPP_MSG_SYNC_WORD;

    REQUIRE(SppHostProcessMessage(&spphe, msg_short, short_len) == SPP_STATUS_SHORT_MESSAGE);

    uint8_t msg[full_len];
    msg[0] = SPP_MSG_SYNC_WORD;
    msg[1] = 0x00;
    msg[2] = 0;
    msg[3] = 0;
    REQUIRE(SppHostProcessMessage(&spphe, msg, full_len) == SPP_STATUS_UNKNOWN_MSG_ID);
}

TEST_CASE( "spp connect no client" ) {
    setup();
    const uint8_t prop_list_id = 0x11;
    const uint8_t connect_msg_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_CONNECT_REQUEST_BC_SIZE; 
    uint8_t connect_msg[connect_msg_len];
    connect_msg[0] = SPP_MSG_SYNC_WORD;
    connect_msg[1] = SPP_MSG_CONNECT_REQUEST_BC_ID;
    connect_msg[2] = c0_address;
    connect_msg[4] = host_address;
    connect_msg[5] = prop_list_id;

    REQUIRE(SppHostProcessMessage(&spphe, connect_msg, connect_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.state == SPP_STATE_CONNECTING);
    REQUIRE(last_msg_len == SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_CONNECT_RESPONSE_SIZE);
    REQUIRE(spphe.address_buffer[0] == c0_address);
    REQUIRE(last_msg[0] == SPP_MSG_SYNC_WORD);
    REQUIRE(last_msg[1] == SPP_MSG_CONNECT_RESPONSE_ID);
    REQUIRE(last_msg[2] == host_address);
    REQUIRE(last_msg[3] == c0_address);
    REQUIRE(last_msg[4] == SPP_STATUS_OK);
    
    REQUIRE(spphe.client_count == 1);
    REQUIRE(*(uint8_t*)(spphe.clients[0].address) == c0_address);
    
    REQUIRE(SppHostProcessMessage(&spphe, connect_msg, connect_msg_len) == SPP_STATUS_OK);
    REQUIRE(last_msg[4] == SPP_STATUS_BUSY);
    REQUIRE(spphe.client_count == 1);
    REQUIRE(*(uint8_t*)(spphe.clients[0].address) == c0_address);

    const uint8_t prop_def_resp_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_PROP_LIST_RESPONSE_SIZE;
    uint8_t prop_def_resp_msg[prop_def_resp_len];
    prop_def_resp_msg[0] = SPP_MSG_SYNC_WORD;
    prop_def_resp_msg[1] = SPP_MSG_PROP_LIST_RESPONSE_ID;
    prop_def_resp_msg[2] = c0_address;
    prop_def_resp_msg[3] = host_address;
    prop_def_resp_msg[4] = 1;
    prop_def_resp_msg[5] = sizeof(uint16_t);

    REQUIRE(SppHostProcessMessage(&spphe, prop_def_resp_msg, prop_def_resp_len) == SPP_STATUS_OK);
   
    const char name[6] = "prop1";
    const uint8_t prop_def_msg_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_PROP_DEF_SIZE + strlen(name) + 1;
    uint8_t prop_def_msg[prop_def_msg_len];

    const uint16_t id = 0x8C;
    SppPropertyFlags_t flags;
    flags.is_readable = 1;
    flags.is_writeable = 0;
    flags.is_read_streamable = 0;
    flags.is_write_streamable = 1;
    prop_def_msg[0] = SPP_MSG_SYNC_WORD;
    prop_def_msg[1] = SPP_MSG_PROP_DEF_ID;
    prop_def_msg[2] = c0_address;
    prop_def_msg[3] = host_address;
    prop_def_msg[4] = sizeof(uint16_t);
    prop_def_msg[5] = 0;
    memcpy(prop_def_msg + 6, &id, sizeof(uint16_t));
    prop_def_msg[8] = SPP_PROP_T_U16;
    prop_def_msg[9] = flags.value;
    memcpy(&prop_def_msg[10], name, strlen(name) + 1);

    REQUIRE(SppHostProcessMessage(&spphe, prop_def_msg, prop_def_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.defs_buffer_used == 1);
    REQUIRE(spphe.defs_buffer[0].size == sizeof(uint16_t));
    REQUIRE(spphe.defs_buffer[0].id == id);
    REQUIRE(spphe.defs_buffer[0].flags.is_readable == flags.is_readable);
    REQUIRE(spphe.defs_buffer[0].flags.is_writeable == flags.is_writeable);
    REQUIRE(spphe.defs_buffer[0].flags.is_read_streamable == flags.is_read_streamable);
    REQUIRE(spphe.defs_buffer[0].flags.is_write_streamable == flags.is_write_streamable);
    REQUIRE(spphe.state == SPP_STATE_READY);
    REQUIRE(!strcmp(spphe.defs_buffer[0].name, name));

    SppStream_t s;
    REQUIRE(SppHostStartStream(&spphe, &c0, id, 1200, SPP_STREAM_READ, &s) == SPP_STATUS_OK);
    REQUIRE(s.def->id == id);
    REQUIRE(s.def == spphe.defs_buffer);
    REQUIRE(spphe.pending_request_count == 1);
    REQUIRE(spphe.token_count == 1);

    const uint8_t stream_resp_msg_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_STREAM_RESPONSE_SIZE;
    uint8_t stream_resp_msg[stream_resp_msg_len];
    stream_resp_msg[0] = SPP_MSG_SYNC_WORD;
    stream_resp_msg[1] = SPP_MSG_STREAM_RESPONSE_ID;
    stream_resp_msg[2] = c0_address;
    stream_resp_msg[3] = host_address;
    stream_resp_msg[4] = 1;
    stream_resp_msg[5] = SPP_STATUS_OK;
    stream_resp_msg[6] = sizeof(uint16_t);


    REQUIRE(SppHostProcessMessage(&spphe, stream_resp_msg, stream_resp_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.pending_request_count == 0);
    REQUIRE(spphe.token_count == 1);

    connect_msg[2] = c1_address;

    REQUIRE(SppHostProcessMessage(&spphe, connect_msg, connect_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.client_count == 2);
    REQUIRE(spphe.state == SPP_STATE_READY);
    REQUIRE(last_msg_len == SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_CONNECT_RESPONSE_SIZE);
    REQUIRE(spphe.property_def_start[0] == 0);
    REQUIRE(spphe.property_def_start[1] == 0);
    REQUIRE(spphe.property_count[0] == 1);
    REQUIRE(spphe.property_count[1] == 1);

    REQUIRE(spphe.address_buffer[1] == c1_address);
    REQUIRE(last_msg[0] == SPP_MSG_SYNC_WORD);
    REQUIRE(last_msg[1] == SPP_MSG_CONNECT_RESPONSE_ID);
    REQUIRE(last_msg[2] == host_address);
    REQUIRE(last_msg[3] == c1_address);
    REQUIRE(last_msg[4] == SPP_STATUS_DUPLICATE_CLIENT);
    
    connect_msg[2] = c2_address;

    REQUIRE(SppHostProcessMessage(&spphe, connect_msg, connect_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.client_count == 3);
    REQUIRE(spphe.state == SPP_STATE_READY);
    REQUIRE(last_msg_len == SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_CONNECT_RESPONSE_SIZE);
    REQUIRE(spphe.property_def_start[0] == 0);
    REQUIRE(spphe.property_def_start[1] == 0);
    REQUIRE(spphe.property_def_start[2] == 0);
    REQUIRE(spphe.property_count[0] == 1);
    REQUIRE(spphe.property_count[1] == 1);
    REQUIRE(spphe.property_count[2] == 1);

    SppPropertyDefinition_t *dtmp;
    REQUIRE(SppHostGetDefinition(&spphe, &c0, id, &dtmp) == SPP_STATUS_OK);
    REQUIRE(SppHostGetDefinition(&spphe, &c1, id, &dtmp) == SPP_STATUS_OK);
    REQUIRE(SppHostGetDefinition(&spphe, &c2, id, &dtmp) == SPP_STATUS_OK);

    REQUIRE(spphe.address_buffer[2] == c2_address);
    REQUIRE(last_msg[0] == SPP_MSG_SYNC_WORD);
    REQUIRE(last_msg[1] == SPP_MSG_CONNECT_RESPONSE_ID);
    REQUIRE(last_msg[2] == host_address);
    REQUIRE(last_msg[3] == c2_address);
    REQUIRE(last_msg[4] == SPP_STATUS_DUPLICATE_CLIENT);

    REQUIRE(SppHostGetValue(&spphe, &c0, id) == SPP_STATUS_OK);
    REQUIRE(last_msg[0] == SPP_MSG_SYNC_WORD);
    REQUIRE(last_msg[1] == SPP_MSG_GET_REQUEST_ID);
    REQUIRE(last_msg[2] == host_address);
    REQUIRE(last_msg[3] == c0_address);
    REQUIRE(last_msg[4] == spphe.token_count);
    REQUIRE(spphe.pending_request_count == 1);
    REQUIRE(spphe.pending_request_tokens[0] == last_msg[4]);
    REQUIRE(*(uint16_t*)(last_msg + 5) == id);

    REQUIRE(SppHostGetValue(&spphe, &c1, id) == SPP_STATUS_OK);
    REQUIRE(spphe.pending_request_tokens[1] == last_msg[4]);
    REQUIRE(SppHostGetValue(&spphe, &c2, id) == SPP_STATUS_OK);
    REQUIRE(spphe.pending_request_tokens[2] == last_msg[4]);
    REQUIRE(spphe.pending_request_count == 3);

    const uint8_t get_value_resp_msg_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_GET_RESPONSE_BASE_SIZE + sizeof(uint16_t);
    uint8_t get_value_resp_msg[get_value_resp_msg_len];
    get_value_resp_msg[0] = SPP_MSG_SYNC_WORD;
    get_value_resp_msg[1] = SPP_MSG_GET_RESPONSE_ID;
    get_value_resp_msg[2] = c0_address;
    get_value_resp_msg[3] = host_address;
    get_value_resp_msg[4] = spphe.pending_request_tokens[1];
    get_value_resp_msg[5] = SPP_STATUS_OK;
    get_value_resp_msg[6] = sizeof(uint16_t);
    *((uint16_t*)(get_value_resp_msg + 7)) = id;
    *((uint16_t*)(get_value_resp_msg + 9)) = 121;

    REQUIRE(SppHostProcessMessage(&spphe, get_value_resp_msg, get_value_resp_msg_len) == SPP_STATUS_OK);
    REQUIRE(*((uint16_t*)last_value) == 121);
    REQUIRE(spphe.pending_request_count == 2);

    get_value_resp_msg[2] = c2_address;
    get_value_resp_msg[4] = spphe.pending_request_tokens[3];
    *((uint16_t*)(get_value_resp_msg + 7)) = id;
    *((uint16_t*)(get_value_resp_msg + 9)) = 122;
    REQUIRE(SppHostProcessMessage(&spphe, get_value_resp_msg, get_value_resp_msg_len) == SPP_STATUS_OK);
    REQUIRE(*((uint16_t*)last_value) == 122);
    REQUIRE(spphe.pending_request_count == 1);

    uint16_t new_value = 191;
    REQUIRE(SppHostSetValue(&spphe, &c1, id, &new_value) == SPP_STATUS_OK);
    REQUIRE(last_msg[0] == SPP_MSG_SYNC_WORD);
    REQUIRE(last_msg[1] == SPP_MSG_SET_REQUEST_ID);
    REQUIRE(last_msg[2] == host_address);
    REQUIRE(last_msg[3] == c1_address);
    REQUIRE(last_msg[4] == spphe.pending_request_tokens[1]);
    REQUIRE(last_msg[5] == sizeof(uint16_t));
    REQUIRE(*((uint16_t*)(last_msg + 6)) == id);
    REQUIRE(*((uint16_t*)(last_msg + 8)) == new_value);

    const uint8_t set_value_resp_msg_len = SPP_MSG_HDR_SIZE(spphe.address_length) + SPP_MSG_SET_REQUEST_BASE_SIZE + sizeof(uint16_t);
    uint8_t set_value_resp_msg[set_value_resp_msg_len];
    set_value_resp_msg[0] = SPP_MSG_SYNC_WORD;
    set_value_resp_msg[1] = SPP_MSG_SET_RESPONSE_ID;
    set_value_resp_msg[2] = host_address;
    set_value_resp_msg[3] = c1_address;
    set_value_resp_msg[4] = last_msg[4];
    set_value_resp_msg[5] = SPP_STATUS_OK;

    REQUIRE(SppHostProcessMessage(&spphe, set_value_resp_msg, set_value_resp_msg_len) == SPP_STATUS_OK);
    REQUIRE(spphe.pending_request_count == 1);
}





