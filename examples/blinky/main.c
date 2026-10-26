#include "hal.h"
#include <stdint.h>

int main(void) {
    fpu_init();
    rcc_init();
    gpio_init_pc13();

    while (1) {
        gpio_toggle_pc13();
        delay(10000000);
    }

    return 0;
}