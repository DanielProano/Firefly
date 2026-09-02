#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    Task *enqueue_waiting[MAX_TASKS];
    Task *dequeue_waiting[MAX_TASKS];
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
bool enqueue_isr(Queue *queue, void *item);
bool dequeue_isr(Queue *queue, void *item);

#endif