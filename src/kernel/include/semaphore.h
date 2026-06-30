#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "task.h"
#include <stdint.h>
typedef struct {
    Task *waiting;
    uint8_t max_count;
    uint8_t count;
} Semaphore;

void semaphore_init(Semaphore *semaphore, uint8_t max_count);
void semaphore_wait(Semaphore *semaphore);
void semaphore_signal(Semaphore *semaphore);

#endif