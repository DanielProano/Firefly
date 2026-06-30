#ifndef QUEUE_H
#define QUEUE_H

#include "stdint.h"
#include "task.h"
typedef struct {
    Task *enqueue_waiting;
    Task *dequeue_waiting;
    void *buffer;
    uint8_t item_size;
    uint8_t depth;
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} Queue;

void queue_init(Queue *queue, void *buffer, uint8_t item_size, uint8_t depth);
void enqueue(Queue *queue, void *item);
void dequeue(Queue *queue, void *item);

#endif