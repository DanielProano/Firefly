#include "rtos_config.h"
#include "task.h"

void scheduler_init(void);
void scheduler_start(void);
void scheduler_tick(void);
Task *scheduler_get_current_task(void);
void scheduler_select_next_task(void);