#include "gpio.h"
#include "stm32f401xc.h"
#include <stdint.h>

/*  Page 158 of Reference manual 
    Moder configs whether pin is input, output, ect.
*/
void gpio_init_pc13(void) {
    /* Reset */
    GPIOC->MODER &= ~(3U << 26);

    uint32_t enable_output_mode = 1U << 26;
    GPIOC->MODER |= enable_output_mode;
}

/*
    Turn on LED
*/
void gpio_set_pc13(void) {
    GPIOC->ODR |= (1U << 13);
}

/*
    Turn off LED
*/
void gpio_clear_pc13(void) {
    GPIOC->ODR &= ~(1U << 13);
}

/*
    Toggle LED
*/
void gpio_toggle_pc13(void) {
    GPIOC->ODR ^= (1U << 13);
}