#ifndef SPP_HOST_API_H
#define SPP_HOST_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../common/spp_proto_def.h"
#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

SPP_STATUS_T SppHostEngineInit(SppHostEngineInitParams_t* init_params, SppHostEngine_t* host);

void SppConnectToClient(SppHostEngine_t *host, SppAddress_t *client);

SPP_STATUS_T SppHostProcessMessage(SppHostEngine_t* host, uint8_t* message, uint16_t len);

SPP_STATUS_T SppHostGetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id);

SPP_STATUS_T SppHostGetDefinition(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, SppPropertyDefinition_t** d);

SPP_STATUS_T SppHostSetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, void* value);

SPP_STATUS_T SppHostStartStream(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, uint16_t period, uint8_t stream_type, SppStream_t* stream);

uint16_t SppHostGetPropertyCount(SppHostEngine_t* host, SppAddress_t* client);

#ifdef __cplusplus
}
#endif

#endif
