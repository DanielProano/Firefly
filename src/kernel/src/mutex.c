#include "mutex.h"
#include "scheduler.h"
#include "port.h"
#include "fault_indicator.h"
#include <stddef.h>

void mutex_init(Mutex *mutex) {
    mutex->is_locked = false;
    mutex->owner = NULL;
    mutex->waiting = NULL;
}

void mutex_lock(Mutex *mutex) {
    while (mutex->is_locked) {
        if (mutex->waiting != NULL) {
            warning_light_scheduler();
        }
        mutex->waiting = current_task;
        current_task->state = BLOCKED;
        port_trigger_context_switch();
    }

    mutex->is_locked = true;
    mutex->owner = current_task;
}

void mutex_unlock(Mutex *mutex) {
    mutex->is_locked = false;
    mutex->owner = NULL;

    if (mutex->waiting != NULL) {
        mutex->waiting->state = READY;
        mutex->waiting = NULL;
        port_trigger_context_switch();
    }
}