#include "queue.h"
#include "scheduler.h"
#include "port.h"
#include "fault_indicator.h"
#include <stddef.h>
#include <string.h>

/*  This RTOS queue implementation
    is intended for situations with 
    one 'publisher' and one 'subscriber'.
    Otherwise, with multiple of each,
    a task could become blocked
    indefinitely if multiple tasks
    encounter a full or empty stack bc.
    only one task can we be saved as waiting
    at a time */

void queue_init(Queue *queue, void *buffer, uint8_t item_size, uint8_t depth) {
    queue->buffer = buffer;
    queue->item_size = item_size;
    queue->depth = depth;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->enqueue_waiting = NULL;
    queue->dequeue_waiting = NULL;
}

/*  It is standard convention for
    head to be where pop occurs &
    tail to be where push occurs.
    This comes from the fact that
    you enter in the back or 'tail'
    of the queue and exit at the front
    or 'head' */

void enqueue(Queue *queue, void *item) {
    /*  When buffer full, save current task,
        block its state, and switch context.
        Hopefully then some task will take
        off the queue in the meantime.
        On return, use while loop to double
        check queue is not full */
    while (queue->count == queue->depth) {
        if (queue->enqueue_waiting != NULL) {
            warning_light_scheduler();
        }
        queue->enqueue_waiting = current_task;
        current_task->state = BLOCKED;
        port_trigger_context_switch();
    }

    /*  Copy bytes into queue, move tail pointer, 
        increase count */
    memcpy((uint8_t*)queue->buffer + (queue->tail * queue->item_size), item, queue->item_size);
    queue->tail = (queue->tail + 1) % queue->depth;
    queue->count += 1;

    /*  If a task exists that is waiting*/
    if (queue->dequeue_waiting != NULL) {
        queue->dequeue_waiting->state = READY;
        queue->dequeue_waiting = NULL;
        port_trigger_context_switch();
    }
}

/* Basically same as enqueue but inverted */
void dequeue(Queue *queue, void *out) {
    while (queue->count == 0) {
        if (queue->dequeue_waiting != NULL) {
            warning_light_scheduler();
        }
        queue->dequeue_waiting = current_task;
        current_task->state = BLOCKED;
        port_trigger_context_switch();
    }

    memcpy(out, (uint8_t*)queue->buffer + (queue->head * queue->item_size), queue->item_size);
    queue->head = (queue->head + 1) % queue->depth;
    queue->count -= 1;

    if (queue->enqueue_waiting != NULL) {
        queue->enqueue_waiting->state = READY;
        queue->enqueue_waiting = NULL;
        port_trigger_context_switch();
    }
}