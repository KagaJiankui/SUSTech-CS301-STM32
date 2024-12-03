#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>

// 定义环形缓冲区结构体
typedef struct {
    uint8_t *buffer;       // 缓冲区指针
    uint16_t head;         // 头指针
    uint16_t tail;         // 尾指针
    uint16_t size;         // 缓冲区大小
    uint16_t count;        // 当前缓冲区中的元素数量
} RingBuf_t;

// 初始化环形缓冲区
void RingBuf_Init(RingBuf_t *rb, uint8_t *buffer, uint16_t size);

// 向环形缓冲区中添加数据
uint16_t RingBuf_Put(RingBuf_t *rb, uint8_t data);

uint16_t RingBuf_PutBatch(RingBuf_t *rb, uint8_t *data, uint8_t bSize);

// 从环形缓冲区中读取数据
uint16_t RingBuf_Get(RingBuf_t *rb, uint8_t *data);

uint16_t RingBuf_GetBatch(RingBuf_t *rb, uint8_t *data, uint8_t bSize);

// 检查环形缓冲区是否为空
uint8_t RingBuf_IsEmpty(RingBuf_t *rb);

// 检查环形缓冲区是否已满
uint8_t RingBuf_IsFull(RingBuf_t *rb);

// 获取环形缓冲区中的元素数量
uint16_t RingBuf_Count(RingBuf_t *rb);

#endif // RINGBUF_H