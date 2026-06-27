#ifndef PORT_H 
#define PORT_H

#include <stm32f401xc.h>
#include <stdint.h>
#include "rtos_config.h"
#include "task.h"

#define PENDSVSET (1U << 28)

void port_init_task(Task *task, uint32_t priority, uint32_t slot, const char* name);
void port_init_task_stack(Task *task, void (*function)(void));
void port_trigger_context_switch(void);

#endif