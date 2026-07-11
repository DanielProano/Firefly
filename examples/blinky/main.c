#include "fpu.h"
#include "rcc.h"
#include "gpio.h"
#include "nvic.h"
#include "fault_indicator.h"
#include <stdint.h>

int main(void) {
    fpu_init();
    rcc_init();
    gpio_init_pc13();
    nvic_init();

    while (1) {
        gpio_toggle_pc13();
        delay(10000000);
    }

    return 0;
}