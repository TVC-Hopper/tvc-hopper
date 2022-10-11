
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
StcpCrc32(uint8_t *buffer, uint16_t size)
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

extern uint8_t *StcpEscape(uint8_t *buffer, uint16_t *size)
{
    uint16_t output_idx = 0;
    
    write_payload_buffer[output_idx++] = STCP_HEADER;

    for (uint16_t i = 0; i < *size; ++i)
    {
        if (STCP_ESCAPE == buffer[i] || STCP_FOOTER == buffer[i] || STCP_HEADER == buffer[i])
        {
            write_payload_buffer[output_idx++] = STCP_ESCAPE;
        }

        write_payload_buffer[output_idx++] = buffer[i];
    }

    write_payload_buffer[output_idx++] = STCP_FOOTER;
    write_payload_buffer[output_idx++] = STCP_FOOTER;

    *size = output_idx;

    return write_payload_buffer;
}

extern uint8_t *StcpUnEscape(uint8_t *buffer, uint16_t *size)
{
    uint16_t output_idx = 0;
    for (uint16_t i = 1; i < *size - 2; ++i)
    {
        if (STCP_ESCAPE == buffer[i])
        {
            read_payload_buffer[output_idx++] = buffer[++i];
        }
        else
        {
            read_payload_buffer[output_idx++] = buffer[i];
        }
    }

    *size = output_idx;

    return read_payload_buffer;
}

extern StcpStatus_t StcpWrite(StcpEngineHandle_t instance, uint8_t *buffer, uint16_t size)
{
    uint8_t *escaped_buffer = StcpEscape(buffer, &size);
    StcpStatus_t status = instance->callbacks.Send(escaped_buffer, size, instance->instance_data);

    return status;
}

extern StcpStatus_t StcpHandleMessage(StcpEngineHandle_t instance, uint8_t *buffer, uint16_t size)
{
    buffer = StcpUnEscape(buffer, &size);
    StcpStatus_t status = instance->callbacks.HandleMessage(buffer, size, instance->instance_data);

    return status;
}

