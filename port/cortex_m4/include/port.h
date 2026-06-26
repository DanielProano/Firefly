#include "task.h"
#include "rtos_config.h"
#include <stm32f401xc.h>
#include <stdint.h>

#define PENDSVSET (1U << 28)

void port_init_task(void);
void port_init_task_stack(Task *task, void (*function)(void), uint32_t priority, uint32_t new_task_slot, const char* name);
void port_trigger_context_switch(void);