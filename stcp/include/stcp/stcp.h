#ifndef STCP_H
#define STCP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define HEADER ((uint8_t)'\x7A')
#define FOOTER ((uint8_t)'\x7A\x7F')
#define ESCAPE ((uint8_t)'\x7A')

#define STATUS_SUCCESS 0
#define STATUS_UNDEFINED_ERROR 1
#define STATUS_CRC_ERROR 2

#define STATE_FUNCTIONAL 0
#define STATE_INACTIVE 1
#define STATE_ERROR 2

    typedef uint8_t STCPState_t;
    typedef uint8_t STCPStatus_t;

    typedef struct STCPCallbacks_s
    {
        STCPStatus_t (*Send)(void *buffer, uint16_t length, void *instance_data);
        STCPStatus_t (*HandleMessage)(void *buffer, uint16_t length, void *instance_data);
    } STCPCallbacks_t;

    typedef struct STCPEngine_s
    {
        STCPCallbacks_t callbacks;
        void *instance_data;
    } STCPEngine_t;

    typedef STCPEngine_t *STCPEngineHandler_t;

    extern uint32_t
    Crc32(uint8_t *buffer, uint16_t length);

    extern STCPStatus_t Write(STCPEngineHandler_t instance, uint8_t *buffer, uint16_t size);

    extern STCPStatus_t Read(STCPEngineHandler_t instance, uint8_t *buffer, uint16_t size);

    extern uint8_t *UnEscape(uint8_t *buffer, uint16_t *size);

    extern uint8_t *Escape(uint8_t *buffer, uint16_t *size);

#ifdef __cplusplus
}
#endif

#endif
