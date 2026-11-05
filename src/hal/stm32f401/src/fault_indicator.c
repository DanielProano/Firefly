#include "fault_indicator.h"
#include "gpio.h"

#define LONG_DELAY 10000000
#define DELAY_TIME 1000000

void delay(volatile uint32_t count) {
    while (count--);
}

void warning_light(void) {
    while (1) {
        gpio_set_pc13();

        delay(LONG_DELAY);

        for (int i = 0; i < 7; i++) {
            gpio_toggle_pc13();

            delay(DELAY_TIME);
        }
    }
}