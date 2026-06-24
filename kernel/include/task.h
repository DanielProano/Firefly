#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS 16
#define TASK_STACK_SIZE 256

/* More Granular States for Debugging*/
typedef enum {
    UNINITIALIZED,
    READY,
    RUNNING,
    BLOCKED_DELAY,
    BLOCKED_MUTEX,
    BLOCKED_SEMAPHORE,
    BLOCKED_QUEUE,
    SUSPENDED
} Task_State;

typedef struct {
    /* For Context Switching*/
    uint32_t    *stack_ptr;
    /*  Bottom of Stack for Error Checking 
        Serves as Stack Canary (0xDEADBEEF)*/
    uint32_t    *stack_base;
    uint32_t    stack_size;
    uint32_t    delay_until;
    Task_State  state;
    uint8_t     priority;
    const char  *name;
} Task;

Task *task_create(void (*function)(void), uint8_t priority, const char *name);

void task_delay(uint32_t ticks);

bool task_state_overflow(Task task);

static Task task_pool[MAX_TASKS];
static uint32_t stack_pool[MAX_TASKS][TASK_STACK_SIZE];