#ifndef SYSTICK_H
#define SYSTICK_H

void systick_init(void);
void SysTick_Handler(void (*tick_callback)(void));

#endif