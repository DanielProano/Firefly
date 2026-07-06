#include "fault_indicator.h"
#include "gpio.h"

void delay(volatile uint32_t count) {
    while (count--);
}

void warning_light(void) {
    while (1) {
        gpio_set_pc13();

        delay(10000000);

        gpio_clear_pc13();

        delay(10000000);

        gpio_set_pc13();

        delay(10000000);
    }
}