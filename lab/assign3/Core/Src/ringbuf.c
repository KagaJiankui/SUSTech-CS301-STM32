#include "ringbuf.h"

void RingBuf_Init(RingBuf_t *rb, uint8_t *buffer, uint16_t size) {
    rb->buffer = buffer;
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
    rb->count = 0;
}

uint16_t RingBuf_Put(RingBuf_t *rb, uint8_t data) {
    if (RingBuf_IsFull(rb)) {
        return 0; // Buffer full
    }
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;
    return 1;
}

uint16_t RingBuf_PutBatch(RingBuf_t *rb, uint8_t *data, uint8_t bSize) {
    uint16_t freeSpace = rb->size - rb->count;
    if (bSize > freeSpace) {
        return 0; // Not enough space
    }

    uint16_t i;
    for (i = 0; i < bSize; i++) {
        rb->buffer[rb->head] = data[i];
        rb->head = (rb->head + 1) % rb->size;
    }
    rb->count += bSize;
    return bSize;
}

uint16_t RingBuf_Get(RingBuf_t *rb, uint8_t *data) {
    if (RingBuf_IsEmpty(rb)) {
        return 0; // Buffer empty
    }
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;
    return 1;
}

uint16_t RingBuf_GetBatch(RingBuf_t *rb, uint8_t *data, uint8_t bSize) {
    if (bSize > rb->count) {
        return 0; // Not enough data
    }

    uint16_t i;
    for (i = 0; i < bSize; i++) {
        data[i] = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % rb->size;
    }
    rb->count -= bSize;
    return bSize;
}

uint8_t RingBuf_IsEmpty(RingBuf_t *rb) {
    return rb->count == 0;
}

uint8_t RingBuf_IsFull(RingBuf_t *rb) {
    return rb->count == rb->size;
}

uint16_t RingBuf_Count(RingBuf_t *rb) {
    return rb->count;
}