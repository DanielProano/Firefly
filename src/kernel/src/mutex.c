#include "mutex.h"
#include "scheduler.h"
#include "port.h"
#include <stdatomic.h>
#include <stddef.h>

void mutex_init(Mutex *mutex) {
    int mask = port_disable_interrupts();
    mutex->is_locked = false;
    mutex->owner = NULL;

    for (int i = 0; i < MAX_TASKS; i++) {
        mutex->waiting[i] = NULL;
    }
    port_enable_interrupts(mask);
}

void mutex_lock(Mutex *mutex) {
    int mask = port_disable_interrupts();

    while (mutex->is_locked) {
        int slot = -1;

        for (int i = 0; i < MAX_TASKS; i++) {
            if (mutex->waiting[i] == NULL) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            port_fault();
        }

        mutex->waiting[slot] = current_task;
        current_task->state = BLOCKED;
        port_enable_interrupts(mask);
        port_trigger_context_switch();
        mask = port_disable_interrupts();
    }

    mutex->is_locked = true;
    mutex->owner = current_task;

    port_enable_interrupts(mask);
}

void mutex_unlock(Mutex *mutex) {
    int mask = port_disable_interrupts();
    mutex->is_locked = false;
    mutex->owner = NULL;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (mutex->waiting[i] != NULL) {
            mutex->waiting[i]->state = READY;
            mutex->waiting[i] = NULL;
            woke_waiter = true;
        }
    }

    port_enable_interrupts(mask);

    if (woke_waiter) {
        port_trigger_context_switch();
    }
}
