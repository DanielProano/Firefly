#include "scheduler.h"
#include "systick.h"
#include "fault_indicator.h"
#include "port.h"
#include "nvic.h"
#include "stm32f401xc.h"
#include <stddef.h>

/* Only scheduler.c needs tick count */
static uint32_t tick_count = 0;
Task *current_task = NULL;

void scheduler_start(void) {
    scheduler_init();
    port_start_first_task();
}

void idle_task(void) {
    while (1) {
        /*  Special ARM Instr
            to put CPU to sleep until 
            next interrupt fires */
        __WFI();
    }
}

void scheduler_init(void) {
    /* Turn on SysTick */
    systick_init();
    /* Set its priority */
    nvic_init();
    /* Don't waste power when nothing ready */
    task_create(idle_task, 0, "idle");
}

/*  Called every 1ms to unblock 
    sleeping tasks, setup by 
    SysTick_Handler */
void scheduler_tick(void) {
    tick_count += 1;

    for (int i = 0; i < MAX_TASKS; i++) {
        Task *cur_task = &task_pool[i];

        if (cur_task->state != UNINITIALIZED && task_state_overflow(*cur_task)) {
            warning_light_init();
        }

        if (cur_task->state == BLOCKED && tick_count >= cur_task->delay_until) {
            cur_task->state = READY;
        }
    }

    port_trigger_context_switch();
}

/*  Used in PendSV after saving the
    cur task context, PendSV Handler
    will then select next task */
void scheduler_select_next_task(void) {
    Task *next_task = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        Task *cur_task = &task_pool[i];
        if (cur_task->state == READY) {
            if (next_task == NULL) {
                next_task = cur_task;
            } else if (cur_task->priority > next_task->priority) {
                next_task = cur_task;
            }
        }
    }

    /*  Should always have a READY Task,
        so shouldn't have to worry about this */
    if (next_task == NULL) {
        return;
    }

    /*  Mark current task as READY and 
        next task as RUNNING */
    if (current_task != NULL && current_task->state == RUNNING) {
        current_task->state = READY;
    }
    
    next_task->state = RUNNING;
    current_task = next_task;
}

void task_delay(uint32_t ticks) {
    current_task->delay_until = tick_count + ticks;
    current_task->state = BLOCKED;

    port_trigger_context_switch();
}
