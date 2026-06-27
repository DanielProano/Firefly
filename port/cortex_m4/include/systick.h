#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f401xc.h"
#include "scheduler.h"

void systick_init(void);
void SysTick_Handler(void);

#endif