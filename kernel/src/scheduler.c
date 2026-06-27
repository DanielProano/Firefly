#include "scheduler.h"
#include "systick.h"
#include "stddef.h"

/* Only scheduler.c needs tick count */
//static uint32_t tick_count = 0;
Task *current_task = NULL;

void scheduler_start(void) {
    scheduler_init();

}

void scheduler_init(void) {
    systick_init();
}

// void scheduler_tick(void) {

// }

// Task *scheduler_get_current_task(void) {
    
// }

// void scheduler_select_next_task(void) {

// }
