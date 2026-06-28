#include "port.h"
#include "rtos_config.h"
#include "stm32f401xc.h"

void port_init_task(Task *task, uint8_t priority, uint32_t slot, const char *name) {
    task->stack_base = stack_pool[slot];
    *task->stack_base = 0xDEADBEEF;

    task->name = name;
    task->delay_until = 0;
    task->priority = priority;
    task->state = READY;
}

void port_init_task_stack(Task *task, void (*function)(void)) {
    uint32_t *sp = task->stack_base + (TASK_STACK_SIZE);
    
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

    *(--sp) = 0x0;  /* S31 */
    *(--sp) = 0x0;  /* S30 */
    *(--sp) = 0x0;  /* S29 */
    *(--sp) = 0x0;  /* S28 */
    *(--sp) = 0x0;  /* S27 */
    *(--sp) = 0x0;  /* S26 */
    *(--sp) = 0x0;  /* S25 */
    *(--sp) = 0x0;  /* S24 */
    *(--sp) = 0x0;  /* S23 */
    *(--sp) = 0x0;  /* S22 */
    *(--sp) = 0x0;  /* S21 */
    *(--sp) = 0x0;  /* S20 */
    *(--sp) = 0x0;  /* S19 */
    *(--sp) = 0x0;  /* S18 */
    *(--sp) = 0x0;  /* S17 */
    *(--sp) = 0x0;  /* S16 */

    /* Software Fake Frame*/

    *(--sp) = 0x0;  /* R11 */
    *(--sp) = 0x0;  /* R10 */
    *(--sp) = 0x0;  /* R9 */
    *(--sp) = 0x0;  /* R8 */
    *(--sp) = 0x0;  /* R7 */
    *(--sp) = 0x0;  /* R6 */
    *(--sp) = 0x0;  /* R5 */
    *(--sp) = 0x0;  /* R4 */

    task->stack_ptr = sp;
}

/*  Turn on PendSV exception, so
    we can save the current task's
    context to memory */
void port_trigger_context_switch(void) {
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}