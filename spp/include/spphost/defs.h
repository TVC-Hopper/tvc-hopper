#ifndef SPP_HOST_DEFS_H
#define SPP_HOST_DEFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../common/spp_proto_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SppHostCallbacks_s {
    bool (*AreAddressesEqual)(SppAddress_t* a, SppAddress_t* b, void* instance_data);
    SPP_STATUS_T (*SendMessage)(uint8_t* msg, uint16_t len, void* instance_data);
    SPP_STATUS_T (*OnValueResponse)(SppAddress_t *client, uint16_t id, void* value, void* instance_data);
    void (*OnStatusResponse)(SPP_STATUS_T status, void* instance_data);
    void (*OnStreamResponse)(uint32_t timestamp, SppStream_t* stream, void* instance_data);
} SppHostCallbacks_t;

typedef struct SppHostEngineInitParams_s {
    SppPropertyDefinition_t* defs_buffer;
    uint8_t* address_buffer;

    uint16_t defs_buffer_size;
    uint8_t address_length;

    SppHostCallbacks_t callbacks;

    SppAddress_t host_address;
    void* instance_data;
} SppHostEngineInitParams_t;

typedef struct SppHostEngine_s {
    uint8_t token_count;
    uint8_t stream_count;

    SppPropertyDefinition_t* defs_buffer;
    uint16_t defs_buffer_used;
    uint16_t defs_buffer_size;

    uint8_t* address_buffer;

    uint8_t address_length;
    SppState_t state;

    uint8_t client_count;
    SppAddress_t clients[SPP_CONNECTIONS_MAX];
    uint16_t property_count[SPP_CONNECTIONS_MAX];
    uint16_t property_def_start[SPP_CONNECTIONS_MAX];

    uint8_t pending_request_tokens[SPP_PENDING_REQUESTS_MAX];
    uint16_t pending_request_count;

    SppStream_t* streams[SPP_STREAM_MAX_COUNT];

    SppHostCallbacks_t callbacks;

    SppAddress_t host_address;
    void* instance_data;
} SppHostEngine_t;

#ifdef __cplusplus
}
#endif

#endif
