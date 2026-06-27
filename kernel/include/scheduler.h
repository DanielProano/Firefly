#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "rtos_config.h"
#include "task.h"
#include "stddef.h"

extern uint32_t tick_count;
extern Task *current_task;

void scheduler_init(void);
void scheduler_start(void);
void scheduler_tick(void);
Task *scheduler_get_current_task(void);
void scheduler_select_next_task(void);

#endif