

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "api.h"
#include "spp_proto_def.h"

static void SppSendConnectBusyResponse(SppHostEngine_t *host, SppAddress_t *client);
static void SppUnpackPropDef(SppHostEngine_t *host, SppAddress_t *client, uint8_t* message, uint16_t len);
static void SppAddClient(SppHostEngine_t *host, SppAddress_t* client);
static SPP_STATUS_T SppClearToken(SppHostEngine_t *host, uint8_t token);
static SPP_STATUS_T SppInsertToken(SppHostEngine_t *host, uint8_t token);
static uint8_t SppGetToken(SppHostEngine_t *host);
static SPP_STATUS_T SppGetClientIdx(SppHostEngine_t *host, SppAddress_t* list, SppAddress_t* client, uint8_t* idx);
static uint16_t SppFillMessageHeader(uint8_t address_length, uint8_t* buffer, SppAddress_t *host, SppAddress_t* client, uint8_t message_id);

static void SppSendConnectBusyResponse(SppHostEngine_t *host, SppAddress_t *client) {
    // create message
    uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_CONNECT_RESPONSE_SIZE, host->address_length);
    uint8_t msg[msg_size];

    // fill header
    uint16_t body_idx = SppFillMessageHeader(host->address_length, msg, &host->host_address, client, SPP_MSG_CONNECT_RESPONSE_ID);

    msg[msg_size - 1] = SPP_STATUS_BUSY;

    host->callbacks.SendMessage(msg, msg_size, host->instance_data);
}

static void SppUnpackPropDef(SppHostEngine_t *host, SppAddress_t *client, uint8_t* message, uint16_t len) {
    uint16_t msg_idx = 0;
   
    // unpack received
    uint8_t prop_size = message[msg_idx++];
    uint8_t props_remaining = message[msg_idx++];
    uint16_t prop_id = *((uint16_t*)(message + msg_idx));
    msg_idx += sizeof(uint16_t);
    uint8_t prop_type = message[msg_idx++];
    uint8_t prop_flags = message[msg_idx++];

    uint8_t addr_idx;
    SppGetClientIdx(host, host->clients, client, &addr_idx);
    ++host->property_count[addr_idx];

    // save propery definition
    SppPropertyDefinition_t *pd = &host->defs_buffer[host->defs_buffer_used++];
    pd->size = prop_size;
    pd->id = prop_id;
    pd->type = prop_type;
    pd->flags.value = prop_flags;

    // handle name
    uint8_t name_length = len - msg_idx;
    memcpy(pd->name, message + msg_idx, name_length);
    pd->name_length = name_length;

    printf("SPP: [C%d] Got property %s [%d] of size %d\n", addr_idx, pd->name, pd->id, pd->size);

    // not ready until all properties are received
    if (props_remaining == 0) {
        host->state = SPP_STATE_READY;
    }
}

void SppConnectToClient(SppHostEngine_t *host, SppAddress_t *client) {
    SppAddClient(host, client);

    // create message
    uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_CONNECT_RESPONSE_SIZE, host->address_length);
    uint8_t msg[msg_size];

    // fill header
    uint16_t body_idx = SppFillMessageHeader(host->address_length, msg, &host->host_address, client, SPP_MSG_CONNECT_RESPONSE_ID);
    msg[msg_size - 1] = SPP_STATUS_OK;

    // check for duplicate clients
    for (uint8_t i = 0; i < host->client_count; ++i) {
        if (host->clients[i].property_list_id == client->property_list_id) {
            // already just added this client to the list so ensure we don't detect self
            if (!host->callbacks.AreAddressesEqual(&host->clients[i], client, host->instance_data)) {
                host->state = SPP_STATE_READY;
                host->property_def_start[host->client_count - 1] = host->property_def_start[i];
                host->property_count[host->client_count - 1] = host->property_count[i];

                // mark as duplicate
                msg[msg_size - 1] = SPP_STATUS_DUPLICATE_CLIENT;
            }
        }
    }

    host->callbacks.SendMessage(msg, msg_size, host->instance_data);
    
}

static void SppAddClient(SppHostEngine_t *host, SppAddress_t *client) {
    
    // if client already exists, don't add
    for (uint8_t i = 0; i < host->client_count; ++i) {
        if (host->callbacks.AreAddressesEqual(client, &host->clients[i], host->instance_data)) {
            return;
        }
    }

    uint8_t *permanent_addr = &host->address_buffer[host->client_count];

    // put address in permanent location
    memcpy(permanent_addr, client->address, host->address_length);
    client->address = permanent_addr;
    host->clients[host->client_count++] = *client;
}

static SPP_STATUS_T SppClearToken(SppHostEngine_t *host, uint8_t token) {

    // find token
    for (uint8_t i = 0; i < SPP_PENDING_REQUESTS_MAX; ++i) {
        if (host->pending_request_tokens[i] == token) {
            // mark as unused
            host->pending_request_tokens[i] = SPP_TOKEN_MAX_RESERVED;
            --host->pending_request_count;
            return SPP_STATUS_OK;
        }
    }

    return SPP_STATUS_UNKNOWN_REQUEST;
}

static SPP_STATUS_T SppInsertToken(SppHostEngine_t *host, uint8_t token) {
    // find next available token
    for (uint8_t i = 0; i < SPP_PENDING_REQUESTS_MAX; ++i) {
        if (host->pending_request_tokens[i] == SPP_TOKEN_MAX_RESERVED) {

            // save pending
            host->pending_request_tokens[i] = token;
            ++host->pending_request_count;
            return SPP_STATUS_OK;
        }
    }

    return SPP_STATUS_PENDING_FULL;
}

static uint8_t SppGetToken(SppHostEngine_t *host) {

    // create token
    ++host->token_count;

    // rollover is ok
    if (host->token_count == SPP_TOKEN_MAX_RESERVED) {
        host->token_count = 0;
    }
    return host->token_count;
}

static uint16_t SppFillMessageHeader(uint8_t address_length, uint8_t* buffer, SppAddress_t *host, SppAddress_t* client, uint8_t message_id) {
    // [0]: sync word
    // [1]: message id
    // [2:n] host address
    // [2+n:2+2n] client address
    buffer[0] = SPP_MSG_SYNC_WORD;
    buffer[1] = message_id;
    memcpy(&buffer[2], host->address, address_length);
    memcpy(&buffer[2 + address_length], client->address, address_length);

    // size of header
    return MESSAGE_SIZE(0, address_length);
}


static SPP_STATUS_T SppGetClientIdx(SppHostEngine_t *host, SppAddress_t* list, SppAddress_t* client, uint8_t* idx) {
    // find client match
    for (uint8_t i = 0; i < SPP_CONNECTIONS_MAX; ++i) {
        if (host->callbacks.AreAddressesEqual(list + i, client, host->instance_data)) {
            *idx = i;
            return SPP_STATUS_OK;
        }
    }
    printf("SPP: did not find client\n");
    return SPP_STATUS_NO_SUCH_CLIENT;
}

SPP_STATUS_T SppHostEngineInit(SppHostEngineInitParams_t* init_params, SppHostEngine_t* host) {
    host->token_count = 0;
    host->defs_buffer = init_params->defs_buffer;
    host->defs_buffer_used = 0;
    host->defs_buffer_size = init_params->defs_buffer_size;
    host->client_count = 0;
    host->address_length = init_params->address_length;
    host->address_buffer = init_params->address_buffer;
    host->state = SPP_STATE_READY;

    // clear any exisitng data
    for (uint8_t i = 0; i < SPP_CONNECTIONS_MAX; ++i) {
        host->property_count[i] = 0;
        host->property_def_start[i] = 0;
        host->clients[i].address = NULL;
    }

    // clear any exisitng data
    for (uint16_t i = 0; i < SPP_PENDING_REQUESTS_MAX; ++i) {
        host->pending_request_tokens[i] = SPP_TOKEN_MAX_RESERVED;
    }

    host->pending_request_count = 0;
    host->callbacks = init_params->callbacks;
    host->host_address = init_params->host_address;
    host->instance_data = init_params->instance_data;

    return SPP_STATUS_OK;
}

SPP_STATUS_T SppHostProcessMessage(SppHostEngine_t* host, uint8_t* message, uint16_t len) {

    uint8_t addr_len = host->address_length;
   
    // messages must have at least 4 bytes
    if (len < (addr_len * 2) + 2) {
        return SPP_STATUS_SHORT_MESSAGE;
    }

    // check sync word
    uint8_t sync_word = message[0];
    if (sync_word != SPP_MSG_SYNC_WORD) {
        return SPP_STATUS_INVALID_SYNC_WORD;
    }

    // unpack msg id
    uint8_t msg_id = message[1];

    // unpack addresses
    uint8_t host_address[addr_len];
    uint8_t client_address[addr_len];
    memcpy(host_address, &message[2], addr_len);
    memcpy(client_address, &message[2 + addr_len - 1], addr_len);

    // temporary
    SppAddress_t client = {
        .address = client_address,
    };

    // continue parsing at end of header
    uint16_t body_idx = SPP_MSG_HDR_SIZE(host->address_length);

    // potentially have a valid message
    if (SPP_MSG_CONNECT_REQUEST_BC_ID == msg_id) {
        // don't accept new client if busy
        if (host->state != SPP_STATE_READY) {
            SppSendConnectBusyResponse(host, &client);
            printf("SPP: busy\n");
            return SPP_STATUS_OK;
        }
        
        host->state = SPP_STATE_CONNECTING;
        client.property_list_id = *((uint16_t*)(message + body_idx));
        SppConnectToClient(host, &client);

    } else if (SPP_MSG_PROP_LIST_RESPONSE_ID == msg_id) {

        // check host can handle property count
        uint8_t num_props = message[body_idx++];
        uint16_t num_bytes = *((uint16_t*)(message + body_idx));
        if (num_props + host->defs_buffer_used > host->defs_buffer_size) {
            printf("SPP: too many properties\n");
            return SPP_STATUS_TOO_MANY_PROPERTIES;
        }

    } else if (SPP_MSG_PROP_DEF_ID == msg_id) {
        // unpack received property
        SppUnpackPropDef(host, &client, &message[body_idx], len - body_idx);
    } else if (SPP_MSG_GET_RESPONSE_ID == msg_id) {
        // unpack
        uint8_t token = message[body_idx++];
        uint8_t status = message[body_idx++];
        uint8_t size = message[body_idx++];
        uint16_t id = *((uint16_t*)(message + body_idx));
        body_idx += sizeof(uint16_t);
        uint8_t tkn_status = SppClearToken(host, token);

        // token and status should be valid
        if (status) {
            return status;
        }

        if (tkn_status) {
            return tkn_status;
        }
       
        // let application handle received value
        host->callbacks.OnValueResponse(&client, id, &message[body_idx], host->instance_data);
    } else if (SPP_MSG_SET_RESPONSE_ID == msg_id) {
        // unpack
        uint8_t token = message[body_idx++];
        uint8_t status = message[body_idx++];
        uint8_t tkn_status = SppClearToken(host, token);

        // token and status should be valid
        if (status) {
            return status;
        }

        if (tkn_status) {
            return tkn_status;
        }
    } else if (SPP_MSG_STREAM_RESPONSE_ID == msg_id) {
        // unpack
        uint8_t token = message[body_idx++];
        uint8_t status = message[body_idx++];
        uint8_t tkn_status = SppClearToken(host, token);

        // token and status should be valid
        if (status) {
            return status;
        }

        if (tkn_status) {
            return tkn_status;
        }

    } else if (SPP_MSG_STREAM_ID == msg_id) {
        // unpack
        uint8_t stream_id = message[body_idx++];

        SppStream_t* stream = host->streams[stream_id];
        uint32_t timestamp;
        memcpy(&timestamp, message + body_idx, sizeof(timestamp));
        body_idx += sizeof(timestamp);
        stream->value = &message[body_idx];

        // let application handle stream
        host->callbacks.OnStreamResponse(timestamp, stream, host->instance_data);

    } else {
        printf("SPP: unknown message\n");
        return SPP_STATUS_UNKNOWN_MSG_ID;
    }

    return SPP_STATUS_OK;
}

SPP_STATUS_T SppHostGetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id) {
    if (host->pending_request_count > SPP_PENDING_REQUESTS_MAX) {
        return SPP_STATUS_PENDING_FULL;
    }
    
    // check valid client
    uint8_t client_idx;
    SPP_STATUS_T status = SppGetClientIdx(host, host->clients, client, &client_idx);
    if (status) {
        return status;
    }

    // create message
    uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_GET_REQUEST_SIZE, host->address_length);
    uint8_t msg[msg_size];

    // fill header
    uint16_t body_idx = SppFillMessageHeader(host->address_length, msg, &host->host_address, client, SPP_MSG_GET_REQUEST_ID);

    // get new token
    uint8_t token = SppGetToken(host);
    SppInsertToken(host, token);
    msg[body_idx++] = token;
    memcpy(msg + body_idx, &id, sizeof(uint16_t));

    // send request
    host->callbacks.SendMessage(msg, msg_size, host->instance_data);

    return SPP_STATUS_OK;
}

SPP_STATUS_T SppHostGetDefinition(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, SppPropertyDefinition_t** d) {
    uint8_t addr_idx;
    SPP_STATUS_T status = SppGetClientIdx(host, host->clients, client, &addr_idx);

    // check if client valid
    if (status) {
        return status;
    }

    // search for property definition in client range
    SppPropertyDefinition_t* start = &host->defs_buffer[host->property_def_start[addr_idx]];
    for (uint16_t i = 0; i < host->property_count[addr_idx]; ++i) {
        if (start[i].id == id) {
            // set pointer if found
            *d = &start[i];
            return SPP_STATUS_OK;
        }
    }

    printf("SPP: did not find property definition for client [%d]\n", addr_idx);

    return SPP_STATUS_UNKNOWN_PROPERTY;
}

SPP_STATUS_T SppHostSetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, void* value) {
    if (host->pending_request_count > SPP_PENDING_REQUESTS_MAX) {
        return SPP_STATUS_PENDING_FULL;
    }

    uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_SET_REQUEST_BASE_SIZE, host->address_length);

    // find definition
    SppPropertyDefinition_t* def;
    SPP_STATUS_T status = SppHostGetDefinition(host, client, id, &def);
    if (status) {
        return status;
    }
   
    // create message, must include size of value
    uint16_t value_size = def->size;
    msg_size += value_size;
    uint8_t msg[msg_size];
    uint16_t body_idx = SppFillMessageHeader(host->address_length, msg, &host->host_address, client, SPP_MSG_SET_REQUEST_ID);

    // get new token
    uint8_t token = SppGetToken(host);
    SppInsertToken(host, token);

    // fill new message
    msg[body_idx++] = token;
    msg[body_idx++] = value_size;
    memcpy(msg + body_idx, &id, sizeof(uint16_t));
    body_idx += sizeof(uint16_t); 
    memcpy(msg + body_idx, value, value_size);

    host->callbacks.SendMessage(msg, msg_size, host->instance_data);

    return SPP_STATUS_OK;
}

SPP_STATUS_T SppHostStartStream(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, uint16_t period, uint8_t stream_type, SppStream_t* stream) {
    if (host->pending_request_count > SPP_PENDING_REQUESTS_MAX) {
        return SPP_STATUS_PENDING_FULL;
    }

    // check client exists
    uint8_t client_idx;
    SPP_STATUS_T status = SppGetClientIdx(host, host->clients, client, &client_idx);
    if (status) {
        return status;
    }
  
    // check definition exists
    status = SppHostGetDefinition(host, client, id, &stream->def);
    if (status) {
        return status;
    }
    
    // initialize stream object
    stream->period = period;
    stream->stream_id = host->stream_count++;
    host->streams[stream->stream_id] = stream;

    // create message
    uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_STREAM_REQUEST_SIZE, host->address_length);
    uint8_t msg[msg_size];
    uint16_t body_idx = SppFillMessageHeader(host->address_length, msg, &host->host_address, client, SPP_MSG_STREAM_REQUEST_ID);

    uint8_t token = SppGetToken(host);
    SppInsertToken(host, token);
    msg[body_idx++] = token;
    msg[body_idx++] = stream_type;
    msg[body_idx++] = stream->stream_id;
    memcpy(msg + body_idx, &id, sizeof(uint16_t));
    body_idx += sizeof(uint16_t); 
    memcpy(msg + body_idx, &period, sizeof(uint16_t));

    host->callbacks.SendMessage(msg, msg_size, host->instance_data);

    return SPP_STATUS_OK;
}

uint16_t SppHostGetPropertyCount(SppHostEngine_t* host, SppAddress_t* client) {
    for (uint8_t i = 0; i < host->client_count; ++i) {
        // count properties for this client
        if (host->callbacks.AreAddressesEqual(client, &host->clients[i], host->instance_data)) {
            return host->property_count[i];
        }
    }

    return 0;
}

