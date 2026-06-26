#ifndef FAULT_INDICATOR_H
#define FAULT_INDICATOR_H

#include <stdint.h>

void delay(volatile uint32_t count);
void warning_light_init(void);
void warning_light_mutex(void);

#endif