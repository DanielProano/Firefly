#include "task.h"
#include "port.h"

Task task_pool[MAX_TASKS];
uint32_t stack_pool[MAX_TASKS][TASK_STACK_SIZE];

Task *task_create(void (*function)(void), uint8_t priority, const char *name) {
    int mask = port_disable_interrupts();
    int new_task_slot = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool[i].state == UNINITIALIZED) {
            new_task_slot = i;
            break;
        }
    }

    /* Fail if not enough task slots available */
    if (new_task_slot == -1) {
        port_enable_interrupts(mask);
        port_fault();
    }

    Task *new_task = &task_pool[new_task_slot];
    port_init_task(new_task, priority, new_task_slot, name);
    port_init_task_stack(new_task, function);

    port_enable_interrupts(mask);
    return new_task;
}

bool task_state_overflow(Task task) {
    return (*task.stack_base != 0xDEADBEEF) ? true : false;
}