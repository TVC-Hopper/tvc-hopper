#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "api.h"
#include "defs.h"
#include "spp_proto_def.h"

static SPP_STATUS_T SppGetDefinition(SppClientEngine_t *client, uint16_t id, SppPropertyDefinition_t **pdef);
static uint16_t SppFillMessageHeader(uint8_t address_length, uint8_t* buffer, SppAddress_t *host, SppAddress_t* client, uint8_t message_id);
static SPP_STATUS_T SppGetStream(SppClientEngine_t *client, uint16_t stream_id, SppStream_t **s);
static SPP_STATUS_T SppSendPropertyList(SppClientEngine_t *client, uint8_t* message, uint8_t* host_address);

static SPP_STATUS_T SppGetDefinition(SppClientEngine_t *client, uint16_t id, SppPropertyDefinition_t **pdef) {
    // search for definition
    for (uint8_t i = 0; i < client->property_count; ++i) {
        if (client->properties[i].id == id) {
            // set pointer if found
            *pdef = &client->properties[i];
            return SPP_STATUS_OK;
        }
    }

    return SPP_STATUS_UNKNOWN_PROPERTY;
}

static SPP_STATUS_T SppGetStream(SppClientEngine_t *client, uint16_t stream_id, SppStream_t **s) {
    // search for stream
    for (uint8_t i = 0; i < client->stream_count; ++i) {
        if (client->streams[i].stream_id == stream_id) {
            // set pointer if found
            *s = &client->streams[i];
            return SPP_STATUS_OK;
        }
    }

    return SPP_STATUS_ERROR;
}


static uint16_t SppFillMessageHeader(uint8_t address_length, uint8_t* buffer, SppAddress_t *host, SppAddress_t* client, uint8_t message_id) {
    // [0]: sync word
    // [1]: message id
    // [3:n]: client address
    // [3+n:3+2n]: host address
    buffer[0] = SPP_MSG_SYNC_WORD;
    buffer[1] = message_id;
    memcpy(&buffer[2], client->address, address_length);
    memcpy(&buffer[2 + address_length], host->address, address_length);

    return MESSAGE_SIZE(0, address_length);
}

extern void SppClientEngineInit(SppClientEngineInitParams_t* init_params, SppClientEngine_t* spp) {
    spp->client_address = init_params->client_address;
    spp->broadcast_address = init_params->broadcast_address;
    spp->host_address_buffer = init_params->host_address_buffer;
    spp->host_address.address = spp->host_address_buffer;
    spp->property_count = init_params->property_count;
    spp->property_list_id = init_params->property_list_id;
    spp->address_length = init_params->address_length;
    spp->properties = init_params->properties;
    spp->callbacks = init_params->callbacks;
    spp->instance_data = init_params->instance_data;
    spp->stream_count = 0;
}

void SppClientStart(SppClientEngine_t* client) {
    const uint8_t msg_len = SPP_MSG_HDR_SIZE(client->address_length) + SPP_MSG_CONNECT_REQUEST_BC_SIZE;
    uint8_t connect_msg[msg_len];
   
    // setup connect message
    // broadcast presence
    uint8_t body_idx = SppFillMessageHeader(client->address_length, connect_msg, &client->broadcast_address, &client->client_address, SPP_MSG_CONNECT_REQUEST_BC_ID);
    memcpy(connect_msg + body_idx, &client->property_list_id, sizeof(uint16_t));
    
    // send connect message
    client->callbacks.Send(connect_msg, msg_len, client->instance_data);
}

SPP_STATUS_T SppSendPropertyList(SppClientEngine_t *client, uint8_t* message, uint8_t* host_address) {
    uint8_t addr_len = client->address_length;

    memcpy(client->host_address_buffer, host_address, client->address_length);
    uint8_t status =  message[SPP_MSG_HDR_SIZE(addr_len)];

    if(SPP_STATUS_DUPLICATE_CLIENT == status) {
        // don't send property list again if duplicate client
        client->is_duplicate = true;
        return SPP_STATUS_DUPLICATE_CLIENT;
    } else if (SPP_STATUS_BUSY == status) {
        // don't interrupt ongoing operation (sending list to a client)
        return SPP_STATUS_BUSY;
    } else {
        // send property list
        
        // total property list value size
        uint16_t num_bytes = 0;
        for (uint8_t i = 0; i < client->property_count; ++i) {
            num_bytes += client->properties[i].size;
        }
       
        // create message
        uint8_t prop_response_len = SPP_MSG_HDR_SIZE(addr_len) + SPP_MSG_PROP_LIST_RESPONSE_SIZE;
        uint8_t prop_response_msg[prop_response_len];
        
        // fill header
        uint8_t body_idx = SppFillMessageHeader(addr_len, prop_response_msg, &client->host_address, &client->client_address, SPP_MSG_PROP_LIST_RESPONSE_ID);
        
        // [0]: property count
        // [1:2]: byte count
        prop_response_msg[body_idx++] = client->property_count;
        *((uint16_t*)(prop_response_msg + body_idx)) = num_bytes;

        // send property response
        client->callbacks.Send(prop_response_msg, prop_response_len, client->instance_data);

        // send property definitions
        for (uint8_t i = 0; i < client->property_count; ++i) {
            SppPropertyDefinition_t *pd = &client->properties[i];

            // create message
            uint8_t pdef_msg_len = SPP_MSG_HDR_SIZE(addr_len) + SPP_MSG_PROP_DEF_SIZE + strlen(pd->name) + 1;
            uint8_t pdef_msg[pdef_msg_len];
            
            // fill header
            body_idx = SppFillMessageHeader(addr_len, pdef_msg, &client->host_address, &client->client_address, SPP_MSG_PROP_DEF_ID);

            uint16_t props_remaining = client->property_count - i - 1;

            // property size
            pdef_msg[4] = pd->size;
           
            // don't care if more than 0xFF properties remaining
            pdef_msg[5] = props_remaining > 0xFF ? 0xFF : props_remaining;

            // id
            // type
            // flags
            // name with null terminator
            memcpy(pdef_msg + 6, &pd->id, sizeof(uint16_t));
            pdef_msg[8] = pd->type;
            pdef_msg[9] = pd->flags.value;
            memcpy(&pdef_msg[10], pd->name, strlen(pd->name) + 1);

            client->callbacks.Send(pdef_msg, pdef_msg_len, client->instance_data);
        }
    }
    
    return SPP_STATUS_OK;
}

SPP_STATUS_T SppClientProcessMessage(SppClientEngine_t* client, uint8_t* message, uint16_t len) {
    if (len < SPP_MSG_HDR_SIZE(client->address_length)) {
        return SPP_STATUS_SHORT_MESSAGE;
    }

    uint8_t sync = message[0];
    uint8_t msg_id = message[1];
   
    if (SPP_MSG_SYNC_WORD != sync) {
        return SPP_STATUS_INVALID_SYNC_WORD;
    }
    
    uint8_t addr_len = client->address_length;
    uint8_t host_address[addr_len];
    uint8_t client_address[addr_len];
    uint8_t body_idx = SPP_MSG_HDR_SIZE(addr_len);

    memcpy(client_address, &message[2], addr_len);
    memcpy(host_address, &message[2 + addr_len - 1], addr_len);

    if (SPP_MSG_CONNECT_RESPONSE_ID == msg_id) {
        return SppSendPropertyList(client, message, host_address);
    } else if (SPP_MSG_GET_REQUEST_ID == msg_id) {
	// unpack recieved
	uint16_t token = message[body_idx++];
	uint16_t id = *((uint16_t*)(message + body_idx));

	SppPropertyDefinition_t *pd;
        SppGetDefinition(client, id, &pd);

	// create message
        uint16_t get_resp_msg_len = SPP_MSG_HDR_SIZE(addr_len) + pd->size + SPP_MSG_GET_RESPONSE_BASE_SIZE;
        uint8_t get_resp_msg[get_resp_msg_len];

        // fill header
        body_idx = SppFillMessageHeader(addr_len, get_resp_msg, &client->host_address, &client->client_address, SPP_MSG_GET_RESPONSE_ID);

        // token
        // status
        // value size
        // id
        get_resp_msg[body_idx++] = token;
        get_resp_msg[body_idx++] = SPP_STATUS_OK;
        get_resp_msg[body_idx++] = pd->size;
        *((uint16_t*)(get_resp_msg + body_idx)) = id;
        body_idx += 2;

        // fill buffer
        client->callbacks.GetValue(id, get_resp_msg + body_idx, client->instance_data);

        // send message
        client->callbacks.Send(get_resp_msg, get_resp_msg_len, client->instance_data);

    } else if (SPP_MSG_SET_REQUEST_ID == msg_id) {
        // create message
        uint8_t set_resp_msg_len = SPP_MSG_HDR_SIZE(addr_len) + SPP_MSG_SET_RESPONSE_SIZE;
        uint8_t set_resp_msg[set_resp_msg_len];

        // unpack received
        uint16_t token = message[body_idx++];
        uint16_t size = message[body_idx++];
        uint16_t id = *((uint16_t*)(message + body_idx));
        body_idx += sizeof(uint16_t);
        
        // set property with received value
        client->callbacks.SetValue(id, message + body_idx, client->instance_data);

        // send OK response
        body_idx = SppFillMessageHeader(addr_len, set_resp_msg, &client->host_address, &client->client_address, SPP_MSG_SET_RESPONSE_ID);
        set_resp_msg[body_idx++] = token;
        set_resp_msg[body_idx] = SPP_STATUS_OK;

        client->callbacks.Send(set_resp_msg, set_resp_msg_len, client->instance_data);

    } else if (SPP_MSG_STREAM_REQUEST_ID == msg_id) {
        // unpack received
        uint8_t token = message[body_idx++];
        uint8_t stream_type = message[body_idx++];
        uint8_t stream_id = message[body_idx++];
        uint16_t id = *((uint16_t*)(message + body_idx));
        body_idx += sizeof(uint16_t);
        uint16_t period = *((uint16_t*)(message + body_idx));
        body_idx += sizeof(uint16_t);

        // get next available stream
        SppStream_t *s = &client->streams[client->stream_count++];
        
        // configure
        SppGetDefinition(client, id, &s->def);
        s->stream_id = stream_id;
        s->stream_type = stream_type;
        s->period = period;
        s->elapsed_time = 0;

    } else if (SPP_MSG_STREAM_ID == msg_id) {
        uint16_t stream_id = message[body_idx++];
        body_idx += sizeof(uint32_t); // skip timestamp

        // get stream
        SppStream_t *s;
        SppGetStream(client, stream_id, &s);
        
        // handle received data
        client->callbacks.SetValue(s->def->id, message + body_idx, client->instance_data);
    }

    return SPP_STATUS_OK;
}


extern void SppProcessStreams(SppClientEngine_t* client, uint32_t timestamp, uint32_t elapsed_time) {

    // check each stream for expired handling time
    for (uint8_t i = 0; i < client->stream_count; ++i) {
        SppStream_t *s = &client->streams[i];
        s->elapsed_time += elapsed_time;

        if (s->elapsed_time > s->period) {
            // need to update

            // create message
            uint16_t msg_size = MESSAGE_SIZE(SPP_MSG_STREAM_BASE_SIZE, client->address_length) + s->def->size;
            uint8_t msg[msg_size];

            // fill header
            uint16_t body_idx = SppFillMessageHeader(client->address_length, msg, &client->host_address, &client->client_address, SPP_MSG_STREAM_ID);
            memcpy(msg + body_idx, &s->stream_id, sizeof(s->stream_id));
            body_idx += sizeof(s->stream_id);
            memcpy(msg + body_idx, &timestamp, sizeof(timestamp));
            body_idx += sizeof(timestamp);
            
            client->callbacks.GetValue(s->def->id, msg + body_idx, client->instance_data);
            body_idx += s->def->size;
            
            // send
            client->callbacks.Send(msg, body_idx, client->instance_data);

            // reset timestamp after handling
            client->streams[i].elapsed_time = 0;
        }
    }
}
