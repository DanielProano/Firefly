#include "fault_indicator.h"
#include "gpio.h"

void delay(volatile uint32_t count) {
    while (count--);
}

void warning_light_init(void) {
    while (1) {
        for (int i = 0; i < 6; i++) {
            gpio_toggle_pc13();

            delay(1000000);
        }

        gpio_toggle_pc13();

        delay(10000000);

        for (int i = 0; i < 4; i++) {
            gpio_toggle_pc13();

            delay(1000000);
        }

        delay(10000000);
    }
}


void warning_light_scheduler(void) {
    while (1) {
        for (int i = 0; i < 6; i++) {
            gpio_toggle_pc13();

            delay(1000000);
        }

        gpio_toggle_pc13();

        delay(10000000);

        for (int i = 0; i < 8; i++) {
            gpio_toggle_pc13();

            delay(1000000);
        }

        delay(10000000);
    }
}