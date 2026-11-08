#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"
#include <stdbool.h>

typedef enum {
    MUTEX_OK,
    MUTEX_BAD_OWNER,
    MUTEX_FAILURE,
} MutexStatus;

typedef struct {
    Task *owner;
    Task *waiting_tasks[MAX_TASKS];
    bool is_locked;
} Mutex;

MutexStatus mutex_init(Mutex *mutex);
MutexStatus mutex_lock(Mutex *mutex);
MutexStatus mutex_unlock(Mutex *mutex);

#endif