#ifndef SPP_CLIENT_API_H
#define SPP_CLIENT_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../common/spp_proto_def.h"
#include "defs.h"


#ifdef __cplusplus
extern "C" {
#endif

void SppClientEngineInit(SppClientEngineInitParams_t* init_params, SppClientEngine_t* spp);

void SppClientStart(SppClientEngine_t* client);

SPP_STATUS_T SppClientProcessMessage(SppClientEngine_t* client, uint8_t* message, uint16_t len);

void SppProcessStreams(SppClientEngine_t* client, uint32_t timestamp, uint32_t elapsed_time);


#ifdef __cplusplus
}
#endif

#endif
