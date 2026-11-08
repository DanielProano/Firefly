#include "mutex.h"
#include "scheduler.h"
#include "port.h"
#include <stdatomic.h>
#include <stddef.h>

MutexStatus mutex_init(Mutex *mutex) {
    if (mutex == NULL) {
        return MUTEX_FAILURE;
    }

    uint32_t mask = port_disable_interrupts();

    mutex->is_locked = false;
    mutex->owner = NULL;

    for (int i = 0; i < MAX_TASKS; i++) {
        mutex->waiting_tasks[i] = NULL;
    }

    port_restore_interrupts(mask);

    return MUTEX_OK;
}

MutexStatus mutex_lock(Mutex *mutex) {
    if (mutex == NULL || current_task == NULL) {
        return MUTEX_FAILURE;
    }

    uint32_t mask = port_disable_interrupts();

    while (mutex->is_locked) {
        int slot = -1;

        // Guaranteed to be at least 1 free spot
        for (int i = 0; i < MAX_TASKS; i++) {
            if (mutex->waiting_tasks[i] == NULL) {
                slot = i;
                break;
            }

            if (mutex->waiting_tasks[i]->id == current_task->id) {
                port_restore_interrupts(mask);
                port_fault();
            }
        }

        if (slot == -1) {
            port_restore_interrupts(mask);
            port_fault();
        }

        mutex->waiting_tasks[slot] = current_task;
        current_task->state = BLOCKED;

        port_restore_interrupts(mask);
        port_trigger_context_switch();
        mask = port_disable_interrupts();
    }

    mutex->is_locked = true;
    mutex->owner = current_task;

    port_restore_interrupts(mask);

    return MUTEX_OK;
}

MutexStatus mutex_unlock(Mutex *mutex) {
    if (mutex == NULL || current_task == NULL) {
        return MUTEX_FAILURE;
    }

    uint32_t mask = port_disable_interrupts();

    if (current_task->id != mutex->owner->id) {
        port_restore_interrupts(mask);
        return MUTEX_BAD_OWNER;
    }
    
    mutex->is_locked = false;
    mutex->owner = NULL;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (mutex->waiting_tasks[i] != NULL) {
            mutex->waiting_tasks[i]->state = READY;
            mutex->waiting_tasks[i] = NULL;
            woke_waiter = true;
        }
    }

    port_restore_interrupts(mask);

    if (woke_waiter) {
        port_trigger_context_switch();
    }

    return MUTEX_OK;
}
