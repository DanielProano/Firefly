#include "port.h"
#include "stm32f401xc.h"
#include <stdint.h>

void port_init(Task *task, uint32_t priority, uint32_t new_task_slot, const char* name) {
    task->stack_base = stack_pool[new_task_slot];
    *task->stack_base = 0xDEADBEEF;

    task->name = name;
    task->delay_until = 0;
    task->priority = priority;
    task->state = READY;

    /* PendSV docs*/
    /* https://www.systemonchips.com/step-by-step-guide-to-cortex-m0-pendsv-exception-handling/ */
    /* Page 226 https://www.st.com/resource/en/programming_manual/pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf */

    SCB->ICSR |= PENDSVSET;

    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0x00);

    //NVIC->ISER |= 
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

void port_trigger_context_switch(void) {
    //
}