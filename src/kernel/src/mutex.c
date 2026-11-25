#include "mutex.h"
#include "scheduler.h"
#include "port.h"
#include <stdatomic.h>
#include <stddef.h>

#define RETURN_IF_NULL(item, failure) { \
    if ((item) == NULL) { \
        return (failure); \
    } \
}

MutexStatus mutex_init(Mutex *in_mutex) {
    RETURN_IF_NULL(in_mutex, MUTEX_FAILURE);

    uint32_t mask = port_disable_interrupts();

    in_mutex->is_locked = false;
    in_mutex->owner = NULL;

    for (int i = 0; i < MAX_TASKS; i++) {
        in_mutex->waiting_tasks[i] = NULL;
    }

    port_restore_interrupts(mask);

    return MUTEX_OK;
}

MutexStatus mutex_lock(Mutex *in_mutex) {
    RETURN_IF_NULL(in_mutex, MUTEX_FAILURE);

    uint32_t mask = port_disable_interrupts();

    while (in_mutex->is_locked) {
        int slot = -1;

        // Guaranteed to be at least 1 free spot
        for (int i = 0; i < MAX_TASKS; i++) {
            if (in_mutex->waiting_tasks[i] == NULL) {
                slot = i;
                break;
            }

            if (in_mutex->waiting_tasks[i]->id == current_task->id) {
                port_restore_interrupts(mask);
                port_fault();
            }
        }

        if (slot == -1) {
            port_restore_interrupts(mask);
            port_fault();
        }

        in_mutex->waiting_tasks[slot] = current_task;
        current_task->state = WAITING;

        port_restore_interrupts(mask);
        port_trigger_context_switch();
        mask = port_disable_interrupts();
    }

    in_mutex->is_locked = true;
    in_mutex->owner = current_task;

    port_restore_interrupts(mask);

    return MUTEX_OK;
}

MutexStatus mutex_unlock(Mutex *in_mutex) {
    RETURN_IF_NULL(in_mutex, MUTEX_FAILURE);

    uint32_t mask = port_disable_interrupts();

    if (current_task->id != in_mutex->owner->id) {
        port_restore_interrupts(mask);
        return MUTEX_BAD_OWNER;
    }
    
    in_mutex->is_locked = false;
    in_mutex->owner = NULL;

    bool woke_waiter = false;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (in_mutex->waiting_tasks[i] != NULL) {
            in_mutex->waiting_tasks[i]->state = READY;
            in_mutex->waiting_tasks[i] = NULL;
            woke_waiter = true;
        }
    }

    port_restore_interrupts(mask);

    if (woke_waiter) {
        port_trigger_context_switch();
    }

    return MUTEX_OK;
}
