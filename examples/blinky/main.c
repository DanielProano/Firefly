#include "hal.h"
#include "stdint.h"
#include "fault_indicator.h"

int main(void) {
    fpu_init();
    rcc_init();
    rcc_enable_gpioc();
    gpio_init_pc13();
    nvic_init();

    while (1) {
        gpio_toggle_pc13();
        delay(10000000);
    }

    return 0;
}