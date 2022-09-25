#include "cbuf.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


extern void CircBufInit(CircBufInitParams_t* params, CircBuf_t* buffer) {
    buffer->buffer = params->buffer;
    buffer->max_size = params->max_size;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = 0;
    buffer->is_full = false;
}

extern bool CircBufPush(CircBuf_t* buffer, uint8_t element) {
    if (buffer->is_full) {
        return false;
    }


    buffer->buffer[buffer->head] = element;

    buffer->head++;
    if (buffer->head >= buffer->max_size) {
        buffer->head = 0;
    }

    buffer->size++;

    if (buffer->size >= buffer->max_size) {
        buffer->is_full = true;
    }

    return true;
}

extern uint8_t CircBufPop(CircBuf_t* buffer) {
    uint8_t element = buffer->buffer[buffer->tail];

    buffer->size--;
    buffer->is_full = false;
    buffer->tail++;

    if (buffer->tail >= buffer->max_size) {
        buffer->tail = 0;
    }

    return element;
}

extern uint8_t CircBufPeek(CircBuf_t* buffer) {
    return buffer->buffer[buffer->head];
}

extern uint16_t CircBufGetSize(CircBuf_t* buffer) {
    return buffer->size;
}

extern bool CircBufIsFull(CircBuf_t* buffer) {
    return buffer->is_full;
}
