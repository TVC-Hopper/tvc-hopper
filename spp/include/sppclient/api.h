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

/**
 *  Initialize SPP client engine
 *  
 *  @param init_params Initialization parameters
 *  @param spp object
 */
void SppClientEngineInit(SppClientEngineInitParams_t* init_params, SppClientEngine_t* spp);

/**
 *  Start SPP client, sends broadcast message. Ensure transport and physical layers are configured
 *  and functional before calling.
 *
 *  @param client object
 */
void SppClientStart(SppClientEngine_t* client);

/**
 *  Handle a complete received packet. Packet must be complete or handling error will occur
 *
 *  @param client object
 *  @param message the received data
 *  @param len length of message
 *  @return SPP_SUCCESS if successful
 */
SPP_STATUS_T SppClientProcessMessage(SppClientEngine_t* client, uint8_t* message, uint16_t len);

/**
 *  Call this periodically to handle streams. Times are in milliseconds
 *
 *  @param client object
 *  @param timestamp current time
 *  @param elapsed_time time since the last call to SppProcessStreams
 */
void SppProcessStreams(SppClientEngine_t* client, uint32_t timestamp, uint32_t elapsed_time);


#ifdef __cplusplus
}
#endif

#endif
