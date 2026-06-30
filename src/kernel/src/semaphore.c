#include "semaphore.h"
#include "scheduler.h"
#include "port.h"
#include <stddef.h>

void semaphore_init(Semaphore *semaphore, uint8_t max_count) {
    semaphore->waiting = NULL;
    semaphore->count = 0;
    semaphore->max_count = max_count;
}

void semaphore_wait(Semaphore *semaphore) {
    while (semaphore->count == 0) {
        semaphore->waiting = current_task;
        current_task->state = BLOCKED;
        port_trigger_context_switch();
    }
    
    semaphore->count -= 1;
}

void semaphore_signal(Semaphore *semaphore) {
    if (semaphore->count < semaphore->max_count) {
        semaphore->count += 1;
    }

    if (semaphore->waiting != NULL) {
        semaphore->waiting->state = READY;
        semaphore->waiting = NULL;
        port_trigger_context_switch();
    }
}