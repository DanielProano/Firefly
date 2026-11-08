#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    QUEUE_OK,
    QUEUE_FAILURE,
} QueueStatus;
typedef struct {
    void *buffer;
    uint8_t item_size;
    uint8_t depth;
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t id;
} Queue;

QueueStatus queue_init(Queue *queue, void *buffer, uint8_t item_size, uint8_t depth);
QueueStatus enqueue_blocking(Queue *queue, const void *item);
QueueStatus dequeue_blocking(Queue *queue, void *out);

#endif