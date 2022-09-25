
#include "stcp.h"

#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#define MAX_PAYLOAD_LENGTH 128

static uint8_t write_payload_buffer[MAX_PAYLOAD_LENGTH];
static uint8_t read_payload_buffer[MAX_PAYLOAD_LENGTH];

extern uint32_t
Crc32(uint8_t *buffer, uint16_t size)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t mask = 0;

    crc = 0xFFFFFFFF;
    for (uint16_t i = 0; i < size; ++i)
    {
        crc = crc ^ buffer[i];
        for (uint8_t j = 0; j < 8; ++j)
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}

extern uint8_t *Escape(uint8_t *buffer, uint16_t *size)
{
    uint16_t output_idx = 0;
    for (uint16_t i = 0; i < *size; ++i)
    {
        if (ESCAPE == buffer[i])
        {
            write_payload_buffer[output_idx++] = ESCAPE;
        }

        write_payload_buffer[output_idx++] = buffer[i];
    }

    *size = output_idx;

    return write_payload_buffer;
}

extern uint8_t *UnEscape(uint8_t *buffer, uint16_t *size)
{
    uint16_t output_idx = 0;
    for (uint16_t i = 0; i < *size; ++i)
    {
        if (ESCAPE == buffer[i])
        {
            read_payload_buffer[output_idx++] = buffer[i++];
        }
        else
        {
            read_payload_buffer[output_idx++] = buffer[i];
        }
    }

    *size = output_idx;

    return read_payload_buffer;
}

extern STCPStatus_t Write(STCPEngineHandler_t instance, uint8_t *buffer, uint16_t size)
{
    uint8_t *escaped_buffer = Escape(buffer, &size);
    STCPStatus_t status = instance->callbacks.Send(escaped_buffer, size, instance->instance_data);

    return status;
}

extern STCPStatus_t HandleMessage(STCPEngineHandler_t instance, uint8_t *buffer, uint16_t size)
{
    buffer = UnEscape(buffer, &size);
    STCPStatus_t status = instance->callbacks.HandleMessage(buffer, size, instance->instance_data);

    return status;
}
