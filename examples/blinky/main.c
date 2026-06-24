#include "rcc.h"
#include "gpio.h"
#include "fpu.h"
#include "nvic.h"
#include "stdint.h"

void delay(volatile uint32_t count) {
    while (count--);
}

int main(void) {
    fpu_init();
    rcc_init();
    rcc_enable_gpioc();
    gpio_init_pc13();
    nvic_init();

    while (1) {
        gpio_toggle_pc13();
        delay(5000000);
    }

    return 0;
}