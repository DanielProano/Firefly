#include "fault_indicator.h"
#include "gpio.h"

#define DELAY_TIME 10000000

void delay(volatile uint32_t count) {
    while (count--);
}

void warning_light(void) {
    while (1) {
        gpio_set_pc13();

        delay(DELAY_TIME);

        gpio_clear_pc13();

        delay(DELAY_TIME);

        gpio_set_pc13();

        delay(DELAY_TIME);
    }
}