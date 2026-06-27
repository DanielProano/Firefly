#include "fpu.h"
#include "stm32f401xc.h"

/*  Enables FPU hardware 
    https://deepbluembedded.com/stm32-fpu-floating-point-unit-enable-disable/ */

void fpu_init(void) {
    SCB->CPACR |= ((3U << 20U) | (3U << 22U));
}