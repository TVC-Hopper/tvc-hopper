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

/**
 *  Initialize SPP host
 *
 *  @param init_params intialization parameters
 *  @param host object
 */
SPP_STATUS_T SppHostEngineInit(SppHostEngineInitParams_t* init_params, SppHostEngine_t* host);

/**
 *  Connect to client at provided address.
 *
 *  @param host object
 *  @param client address of client
 */
void SppConnectToClient(SppHostEngine_t *host, SppAddress_t *client);

/**
 *  Handle a complete received packet. An error will occur if the packet isn't complete
 *
 *  @param host object
 *  @param message data received
 *  @param len length of message in bytes
 *  @return SPP_STATUS_SUCCESS if parsing is successful
 */
SPP_STATUS_T SppHostProcessMessage(SppHostEngine_t* host, uint8_t* message, uint16_t len);

/**
 *  Make a request to get a property value
 *
 *  @param host object
 *  @param client address of client
 *  @param id property id
 *  @return SPP_STATUS_SUCCESS if parsing is successful
 */
SPP_STATUS_T SppHostGetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id);

/**
 *  Make a request to set a property value
 *
 *  @param host object
 *  @param client address of client
 *  @param id property id
 *  @return SPP_STATUS_SUCCESS if parsing is successful
 */
SPP_STATUS_T SppHostSetValue(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, void* value);

/**
 *  Get the property definition of a property
 *
 *  @param host object
 *  @param client address of client
 *  @param id property id
 *  @param d pointer to set with a property definition pointer
 *  @return SPP_STATUS_SUCCESS if parsing is successful
 */
SPP_STATUS_T SppHostGetDefinition(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, SppPropertyDefinition_t** d);

/**
 *  Start a property stream at the given interval (ms)
 *
 *  @param host object
 *  @param client address of client
 *  @param id property id
 *  @param period stream interval in ms
 *  @param stream_type read/write stream
 *  @param stream object to use
 *  @return SPP_STATUS_SUCCESS if parsing is successful
 */
SPP_STATUS_T SppHostStartStream(SppHostEngine_t* host, SppAddress_t* client, uint16_t id, uint16_t period, uint8_t stream_type, SppStream_t* stream);

/**
 *  Get the number of properties
 *
 *  @param host object
 *  @param client address of client
 *  @return property count
 */
uint16_t SppHostGetPropertyCount(SppHostEngine_t* host, SppAddress_t* client);

#ifdef __cplusplus
}
#endif

#endif
