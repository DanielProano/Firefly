#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "rtos_config.h"

/* More Granular States for Debugging*/
typedef enum {
    UNINITIALIZED,
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED
} Task_State;

typedef struct {
    uint32_t    *stack_ptr;
    uint32_t    *stack_base;
    uint32_t    stack_size;
    uint32_t    delay_until;
    Task_State  state;
    uint8_t     priority;
    const char  *name;
} Task;

Task *task_create(void (*function)(void), uint8_t priority, const char *name);
void task_delay(uint32_t ticks);
bool task_state_overflow(Task task);

extern Task task_pool[MAX_TASKS];
extern uint32_t stack_pool[MAX_TASKS][TASK_STACK_SIZE];

#endif