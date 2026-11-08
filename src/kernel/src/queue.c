#include "queue.h"
#include "port.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Helper Macros

#define RETURN_IF_NULL(queue, failure) \
    do { \
        if (queue == NULL) { \
            return failure; \
        } \
    } while(0);

#define RETURN_IF_EMPTY(item, failure) \
    do { \
        if (item == 0) { \
            return failure; \
        } \
    } while(0);

// Private API

static inline bool queue_add(Queue *queue, const void *item) {
    RETURN_IF_NULL(queue, false);
    RETURN_IF_NULL(item, false);

    if (queue->count == queue->depth) {
        return false;
    }

    uint32_t offset = (uint32_t) queue->tail * (uint32_t) queue->item_size;
    uint8_t *slot_addr = (uint8_t *)queue->buffer + offset;
    memcpy(slot_addr, item, queue->item_size);

    queue->count += 1;
    queue->tail = (queue->tail + 1) % queue->depth;

    return true;
}

static inline bool queue_read(Queue *queue, void *out) {
    RETURN_IF_NULL(queue, false);
    RETURN_IF_NULL(out, false);

    if (queue->count == 0) {
        return false;
    }

    uint32_t offset = (uint32_t) queue->head * (uint32_t) queue->item_size;
    uint8_t *slot_addr = (uint8_t *)queue->buffer + offset;
    memcpy(out, slot_addr, queue->item_size);

    queue->count -= 1;
    queue->head = (queue->head + 1) % queue->depth;

    return true;
}

// Public API

QueueStatus queue_init(Queue *queue, void *buffer, uint8_t item_size, uint8_t depth) {
    RETURN_IF_NULL(queue, QUEUE_FAILURE);
    RETURN_IF_NULL(buffer, QUEUE_FAILURE);
    RETURN_IF_EMPTY(item_size, QUEUE_FAILURE);
    RETURN_IF_EMPTY(depth, QUEUE_FAILURE);

    uint32_t mask = port_disable_interrupts();

    queue->buffer = buffer;
    queue->item_size = item_size;
    queue->depth = depth;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;

    port_restore_interrupts(mask);
    return QUEUE_OK;
}

QueueStatus enqueue(Queue *queue, const void *item) {
    RETURN_IF_NULL(queue, QUEUE_FAILURE);
    RETURN_IF_NULL(item, QUEUE_FAILURE);
    
    uint32_t mask = port_disable_interrupts();
    
    bool added = queue_add(queue, item);

    port_restore_interrupts(mask);

    return added ? QUEUE_OK : QUEUE_FAILURE;
}

QueueStatus dequeue(Queue *queue, void *out) {
    RETURN_IF_NULL(queue, QUEUE_FAILURE);
    RETURN_IF_NULL(out, QUEUE_FAILURE);
    
    uint32_t mask = port_disable_interrupts();
    
    bool read = queue_read(queue, out);
    
    port_restore_interrupts(mask);

    return read ? QUEUE_OK : QUEUE_FAILURE;
}

