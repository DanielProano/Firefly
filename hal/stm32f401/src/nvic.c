#include "stm32f401xc.h"
#include "nvic.h"

/*  NVICs main role is to handle interrupts
    https://microcontrollerslab.com/nested-vectored-interrupt-controller-nvic-arm-cortex-m/ */
void nvic_init(void) {
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}