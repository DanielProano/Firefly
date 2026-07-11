#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"
#include <stdbool.h>

typedef struct {
    Task *owner;
    Task *waiting;
    bool is_locked;
} Mutex;

void mutex_init(Mutex *mutex);
void mutex_lock(Mutex *mutex);
void mutex_unlock(Mutex *mutex);

#endif