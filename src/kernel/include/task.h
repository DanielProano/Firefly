#ifndef TASK_H
#define TASK_H

#include "rtos_config.h"
#include "task_types.h"
#include <stdint.h>
#include <stdbool.h>

Task *task_create(void (*function)(void), uint8_t priority, const char *name);
bool task_state_overflow(Task task);

extern Task task_pool[MAX_TASKS];
extern uint32_t stack_pool[MAX_TASKS][TASK_STACK_SIZE];

#endif