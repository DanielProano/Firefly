#include "queue.h"
#include "scheduler.h"
#include "port.h"
#include "stm32f401xc.h"
#include <stddef.h>
#include <string.h>

void queue_init(Queue *queue, void *buffer, uint8_t item_size, uint8_t depth) {
    queue->buffer = buffer;
    queue->item_size = item_size;
    queue->depth = depth;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;

    for (int i = 0; i < MAX_TASKS; i++) {
        queue->enqueue_waiting[i] = NULL;
        queue->dequeue_waiting[i] = NULL;
    }
}

void enqueue(Queue *queue, void *item) {
    for (;;) {
        __disable_irq();

        if (queue->count != queue->depth) {
            break;
        }

        int slot = -1;

        for (int i = 0; i < MAX_TASKS; i++) {
            if (queue->enqueue_waiting[i] == NULL) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            __enable_irq();
            port_fault();
        }

        queue->enqueue_waiting[slot] = current_task;
        current_task->state = BLOCKED;

        __enable_irq();
        port_trigger_context_switch();
    }

    memcpy((uint8_t*)queue->buffer + (queue->tail * queue->item_size), item, queue->item_size);
    queue->tail = (queue->tail + 1) % queue->depth;
    queue->count += 1;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (queue->dequeue_waiting[i] != NULL) {
            queue->dequeue_waiting[i]->state = READY;
            queue->dequeue_waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    __enable_irq();

    if (woke_waiter) {
        port_trigger_context_switch();
    }
}

void dequeue(Queue *queue, void *out) {
    for (;;) {
        __disable_irq();

        if (queue->count != 0) {
            break;
        }

        int slot = -1;

        for (int i = 0; i < MAX_TASKS; i++) {
            if (queue->dequeue_waiting[i] == NULL) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            __enable_irq();
            port_fault();
        }

        queue->dequeue_waiting[slot] = current_task;
        current_task->state = BLOCKED;

        __enable_irq();
        port_trigger_context_switch();
    }

    memcpy(out, (uint8_t*)queue->buffer + (queue->head * queue->item_size), queue->item_size);
    queue->head = (queue->head + 1) % queue->depth;
    queue->count -= 1;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (queue->enqueue_waiting[i] != NULL) {
            queue->enqueue_waiting[i]->state = READY;
            queue->enqueue_waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    __enable_irq();

    if (woke_waiter) {
        port_trigger_context_switch();
    }
}

bool enqueue_isr(Queue *queue, void *item) {
    if (queue->count == queue->depth) {
        return false;
    }

    memcpy((uint8_t*)queue->buffer + (queue->tail * queue->item_size), item, queue->item_size);
    queue->tail = (queue->tail + 1) % queue->depth;
    queue->count += 1;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (queue->dequeue_waiting[i] != NULL) {
            queue->dequeue_waiting[i]->state = READY;
            queue->dequeue_waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    if (woke_waiter) {
        port_trigger_context_switch();
    }

    return true;
}

bool dequeue_isr(Queue *queue, void *out) {
    if (queue->count == 0) {
        return false;
    }

    memcpy(out, (uint8_t*)queue->buffer + (queue->head * queue->item_size), queue->item_size);
    queue->head = (queue->head + 1) % queue->depth;
    queue->count -= 1;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (queue->enqueue_waiting[i] != NULL) {
            queue->enqueue_waiting[i]->state = READY;
            queue->enqueue_waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    if (woke_waiter) {
        port_trigger_context_switch();
    }

    return true;
}
