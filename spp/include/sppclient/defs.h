#ifndef SPP_CLIENT_DEFS_H
#define SPP_CLIENT_DEFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../common/spp_proto_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SppClientCallbacks_s {
    SPP_STATUS_T (*Send)(uint8_t* bytes, uint16_t len, void* instance_data);
    SPP_STATUS_T (*SetValue)(uint16_t id, void* value, void* instance_data);
    SPP_STATUS_T (*GetValue)(uint16_t id, void* value, void* instance_data);
} SppClientCallbacks_t;

typedef struct SppClientEngineInitParams_s {
    SppAddress_t broadcast_address;
    SppAddress_t client_address;
    void* host_address_buffer;
    uint16_t property_count;
    uint16_t property_list_id;
    uint8_t address_length;
    SppPropertyDefinition_t* properties;
    SppClientCallbacks_t callbacks;
    void* instance_data;
} SppClientEngineInitParams_t;

typedef struct SppClientEngine_s {
    SppAddress_t broadcast_address;
    SppAddress_t client_address;
    SppAddress_t host_address;
    void* host_address_buffer;
    uint16_t property_count;
    uint16_t property_list_id;
    uint8_t address_length;
    bool is_duplicate;
    SppPropertyDefinition_t* properties;
    SppClientCallbacks_t callbacks;
    uint8_t stream_count;
    SppStream_t streams[SPP_STREAM_MAX_COUNT];
    void* instance_data;
} SppClientEngine_t;



#ifdef __cplusplus
}
#endif

#endif
