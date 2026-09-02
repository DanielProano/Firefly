#include "semaphore.h"
#include "scheduler.h"
#include "port.h"
#include <stdbool.h>
#include <stddef.h>

void semaphore_init(Semaphore *semaphore, uint8_t max_count) {
    semaphore->count = 0;
    semaphore->max_count = max_count;

    for (int i = 0; i < MAX_TASKS; i++) {
        semaphore->waiting[i] = NULL;
    }
}

void semaphore_wait(Semaphore *semaphore) {
    while (semaphore->count == 0) {
        int slot = -1;

        for (int i = 0; i < MAX_TASKS; i++) {
            if (semaphore->waiting[i] == NULL) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            port_fault();
        }

        semaphore->waiting[slot] = current_task;
        current_task->state = BLOCKED;
        port_trigger_context_switch();
    }

    semaphore->count -= 1;
}

void semaphore_signal(Semaphore *semaphore) {
    if (semaphore->count < semaphore->max_count) {
        semaphore->count += 1;
    }

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (semaphore->waiting[i] != NULL) {
            semaphore->waiting[i]->state = READY;
            semaphore->waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    if (woke_waiter) {
        port_trigger_context_switch();
    }
}
