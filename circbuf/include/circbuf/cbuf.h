#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CircBufInitParams_s {
    uint8_t* buffer;
    uint16_t max_size;
} CircBufInitParams_t;

typedef struct CircBuf_s {
    uint8_t* buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
    uint16_t max_size;
    bool is_full;
} CircBuf_t;

void CircBufInit(CircBufInitParams_t* params, CircBuf_t* buffer);

bool CircBufPush(CircBuf_t* buffer, uint8_t element);

uint8_t CircBufPop(CircBuf_t* buffer);

uint8_t CircBufPeek(CircBuf_t* buffer);

uint16_t CircBufGetSize(CircBuf_t* buffer);

bool CircBufIsFull(CircBuf_t* buffer);

#ifdef __cplusplus
}
#endif

#endif
