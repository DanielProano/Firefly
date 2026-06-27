#ifndef PORT_H 
#define PORT_H

#include <stdint.h>
#include "task.h"

#define PENDSVSET (1U << 28)

/* In port.c */
void port_init_task(Task *task, uint8_t priority, uint32_t slot, const char* name);
void port_init_task_stack(Task *task, void (*function)(void));
void port_trigger_context_switch(void);

/* In port.s */
void PendSV_Handler(void);
void port_start_first_task(void);

#endif