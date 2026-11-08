#ifndef PORT_H 
#define PORT_H

#include "task_types.h"
#include <stdint.h>

#define PENDSVSET (1U << 28)

/* In port.c */
void port_init_task(Task *task, uint8_t priority, uint32_t slot, const char* name);
void port_init_task_stack(Task *task, void (*function)(void));
void port_trigger_context_switch(void);
void port_enable_interrupts(void);
void port_restore_interrupts(uint32_t mask);
uint32_t port_disable_interrupts(void);

/* In port.s */
void PendSV_Handler(void);
void port_start_first_task(void);
void port_fault(void);
void port_idle(void);

#endif