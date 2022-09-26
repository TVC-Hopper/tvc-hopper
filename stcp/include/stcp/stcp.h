#ifndef STCP_H
#define STCP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define STCP_HEADER ((uint8_t)'\x7A')
#define STCP_ESCAPE ((uint8_t)'\x7B')
#define STCP_FOOTER ((uint8_t)'\x7F')

#define STCP_STATUS_SUCCESS 0
#define STCP_STATUS_UNDEFINED_ERROR 1
#define STCP_STATUS_CRC_ERROR 2

#define STCP_STATE_FUNCTIONAL 0
#define STCP_STATE_INACTIVE 1
#define STCP_STATE_ERROR 2

typedef uint8_t StcpState_t;
typedef uint8_t StcpStatus_t;

typedef struct STCPCallbacks_s
{
    StcpStatus_t (*Send)(void *buffer, uint16_t length, void *instance_data);
    StcpStatus_t (*HandleMessage)(void *buffer, uint16_t length, void *instance_data);
} StcpCallbacks_t;

typedef struct StcpEngine_s
{
    StcpCallbacks_t callbacks;
    void *instance_data;
} StcpEngine_t;

typedef StcpEngine_t* StcpEngineHandle_t;

extern uint32_t
StcpCrc32(uint8_t *buffer, uint16_t length);

extern StcpStatus_t StcpHandleMessage(StcpEngineHandle_t instance, uint8_t *buffer, uint16_t size);

extern StcpStatus_t StcpWrite(StcpEngineHandle_t instance, uint8_t *buffer, uint16_t size);

extern uint8_t *StcpUnEscape(uint8_t *buffer, uint16_t *size);

extern uint8_t *StcpEscape(uint8_t *buffer, uint16_t *size);

#ifdef __cplusplus
}
#endif

#endif
