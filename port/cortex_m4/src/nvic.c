#include "nvic.h"
#include "stm32f401xc.h"

/*  NVICs main role is to handle interrupts
    https://microcontrollerslab.com/nested-vectored-interrupt-controller-nvic-arm-cortex-m/ */
void nvic_init(void) {
    /* PendSV docs*/
    /* https://www.systemonchips.com/step-by-step-guide-to-cortex-m0-pendsv-exception-handling/ */
    /* Page 226 https://www.st.com/resource/en/programming_manual/pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf */

    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}