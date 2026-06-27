#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

extern Task *current_task;

void scheduler_init(void);
void scheduler_start(void);
void scheduler_tick(void);
Task *scheduler_get_current_task(void);
void scheduler_select_next_task(void);

#endif