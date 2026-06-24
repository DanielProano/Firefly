#include "task.h"
#include "gpio.h"
#include <stdint.h>

Task *task_create(void (*function)(void), uint8_t priority, const char *name) {
    int new_task_slot = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool[i].state == UNINITIALIZED) {
            new_task_slot = i;
            break;
        }
    }

    /* Fail if not enough task slots available */
    if (new_task_slot == -1) {
        while(1) {
            gpio_toggle_pc13();

            task_delay(3000000);
        }
    }

    Task *new_task = &task_pool[new_task_slot];

    new_task->name = name;
    new_task->stack_base = stack_pool[new_task_slot];
    *new_task->stack_base = 0xDEADBEEF;

    /* Need to Fake a Stake frame*/

    uint32_t *sp = new_task->stack_base + (TASK_STACK_SIZE);
    
    /* Hardware Fake Frame */
    
    *(--sp) = 0x0;  /* FPSCR */
    *(--sp) = 0x0;  /* S15 */
    *(--sp) = 0x0;  /* S14 */
    *(--sp) = 0x0;  /* S13 */
    *(--sp) = 0x0;  /* S12 */
    *(--sp) = 0x0;  /* S11 */
    *(--sp) = 0x0;  /* S10 */
    *(--sp) = 0x0;  /* S9 */
    *(--sp) = 0x0;  /* S8 */
    *(--sp) = 0x0;  /* S7 */
    *(--sp) = 0x0;  /* S6 */
    *(--sp) = 0x0;  /* S5 */
    *(--sp) = 0x0;  /* S4 */
    *(--sp) = 0x0;  /* S3 */
    *(--sp) = 0x0;  /* S2 */
    *(--sp) = 0x0;  /* S1 */
    *(--sp) = 0x0;  /* S0 */
    *(--sp) = 0x01000000; /* XPSR needs thumb bit, its stupid bc. this cant even run non-thumb instrs*/
    *(--sp) = (uint32_t) function; /* PC */
    *(--sp) = 0xFFFFFFFF; /* Linker Register should fault if ever returned */
    *(--sp) = 0x0;  /* R12 */
    *(--sp) = 0x0;  /* R3 */
    *(--sp) = 0x0;  /* R2 */
    *(--sp) = 0x0;  /* R1 */
    *(--sp) = 0x0;  /* R0 */

    /* Software Fake Frame*/

    *(--sp) = 0x0;  /* R11 */
    *(--sp) = 0x0;  /* R10 */
    *(--sp) = 0x0;  /* R9 */
    *(--sp) = 0x0;  /* R8 */
    *(--sp) = 0x0;  /* R7 */
    *(--sp) = 0x0;  /* R6 */
    *(--sp) = 0x0;  /* R5 */
    *(--sp) = 0x0;  /* R4 */

    new_task->stack_ptr = sp;

    new_task->delay_until = 0;
    new_task->priority = priority;
    new_task->state = READY;

    return new_task;
}

void task_delay(uint32_t ticks) {
    ticks += ticks; /* Just using ticks so it compiles */
    //TODO after scheduler
}

bool task_state_overflow(Task task) {
    if (*task.stack_base != 0xDEADBEEF) {
        return true;
    } else {
        return false;
    }
}