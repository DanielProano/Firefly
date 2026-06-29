#include "hal.h"
#include "task.h"
#include "scheduler.h"
#include "fault_indicator.h"
#include <stdint.h>

void task_one(void);
void task_two(void);

int main(void) {
    fpu_init();
    rcc_init();
    rcc_enable_gpioc();
    gpio_init_pc13();

    task_create(&task_one, 1, "task one");
    task_create(&task_two, 2, "task two");

    scheduler_start();

    return 0;
}

void task_one(void) {
    while (1) {
        gpio_toggle_pc13();

        delay(10000000);

        gpio_toggle_pc13();

        delay(10000000);

        gpio_toggle_pc13();

        task_delay(1000);
    }
}

void task_two(void) {
    while (1) {
        gpio_toggle_pc13();

        delay(500000);

        gpio_toggle_pc13();

        delay(500000);

        task_delay(1000);
    }
}