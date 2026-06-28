#include "systick.h"
#include "scheduler.h"
#include "stm32f401xc.h"

void systick_init(void) {
    /* Page 247 in cortex manual*/
    /* Clksource 0 is AHB/8 while 1 is AHB */
    /* Tickint 0 is no Exception request while 1 is*/
    /* Enable 0 is counter disabled, 1 is enabled*/
    SysTick->CTRL |= 
    (
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk |
        SysTick_CTRL_ENABLE_Msk
    );

    /* Page 248 of cortex reference */
    /* Stm runs at 84MHz / sec */
    /* Thats 84,000,000 cycles / sec*/
    /* Convert to cycles/ms */
    /* We get 84,000 cycles / 1 ms */
    /* Standard according to Claude is 1ms */
    /* Subtract 1 for LOAD - 83,999*/
    SysTick->LOAD = 83999;

    /* Page 249 of cortex*/
    /* Just clear it */
    SysTick->VAL = 0x00;
}

void SysTick_Handler(void) {
    scheduler_tick();
}