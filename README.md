# Firefly

A from-scratch, preemptive, priority-based real-time operating system for ARM Cortex-M4, written for the STM32F401/F411. Firefly is the RTOS layer of an autonomous drone's flight controller — it exists to guarantee that the control loop runs on time, every time, regardless of what else the system is doing.

Firefly is part of a larger project: an STM32-based flight controller (running Firefly) paired with a secure bootloader ([Buffalo](../Buffalo)) for verified OTA updates, and an ESP32 acting as a wifi relay for video and update delivery.

## Design goals

- **Deterministic.** The control loop must never miss a deadline. Scheduling is fixed-priority and preemptive — the highest-priority ready task always runs, full stop.
- **Minimal.** No dynamic task creation at runtime beyond startup, no unbounded recursion, no features that don't earn their place in a flight-critical system.
- **Portable in principle, Cortex-M4 in practice.** Scheduling logic and architecture-specific mechanics are deliberately separated (see Architecture), even though Cortex-M4 is the only target today.

## Architecture

Firefly is split into two layers with a hard boundary between them:

```
kernel/      Portable scheduling logic — task states, priority decisions,
             task table, queues, mutexes, semaphores. Architecture-agnostic.

port/
  cortex_m4/ Architecture-specific mechanics — context switching, fake
             stack frame construction, PendSV/SysTick handling.
```

`kernel/` never touches a register directly. Anything that requires knowing this is a Cortex-M4 — stack frame layout, `PendSV`, NVIC priorities for the scheduler's own interrupts — lives in `port/cortex_m4/`. This split means retargeting Firefly to a different architecture would only require a new `port/` implementation; the scheduler's logic wouldn't change.

### Task states

```
UNINITIALIZED -> READY -> RUNNING -> READY (preempted)
                   ^          |
                   +-- BLOCKED / SUSPENDED
```

### Context switching

Firefly uses the standard Cortex-M pattern: `SysTick` fires periodically and makes scheduling decisions; if a switch is needed, it requests one via `PendSV` rather than performing the switch itself. `PendSV` is configured at the lowest interrupt priority, so the actual context switch only happens once everything more urgent has been handled — this avoids ever switching contexts in the middle of handling a higher-priority interrupt.

A task that has never run is made to look, to the restore mechanism, indistinguishable from a task that's merely paused — `port_init_task_stack` constructs a fake exception stack frame ahead of time, including the hardware-saved registers, FPU registers, and the software-saved registers a real switch would expect to find.

### Stack overflow detection

Each task's stack is seeded with a canary value (`0xDEADBEEF`) at its lowest address — the true overflow boundary, since the stack grows downward. `task_state_overflow` checks this value to detect overflow after the fact.

## Project structure

```
Firefly/
|-- kernel/
|   |-- include/   task.h, scheduler.h, queue.h, mutex.h, semaphore.h
|   `-- src/       task.c, scheduler.c, queue.c, mutex.c, semaphore.c
|-- port/
|   `-- cortex_m4/
|       |-- include/  port.h
|       `-- src/      port.c, port.s
|-- hal/
|   `-- stm32f401/  Peripheral drivers (RCC, GPIO, UART, ...)
|-- startup/        Reset handler, vector table
|-- linker/         Linker scripts
|-- config/         rtos_config.h (MAX_TASKS, TASK_STACK_SIZE, ...)
|-- tests/
`-- CMakeLists.txt
```

## Building

Firefly targets `arm-none-eabi` and is built with CMake + Ninja.

### Prerequisites

- ARM GNU Toolchain (`arm-none-eabi-gcc`)
- CMake
- Ninja

### Build

```bash
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
```

Build warnings are treated as errors (`-Wall -Wextra -Werror`) — this is deliberate. A warning in flight-critical embedded code is usually a real bug (shared global state, unused-but-load-bearing values, sign mismatches), not noise, and is treated accordingly.

### Flashing

(Add your flashing tool/command here once finalized -- e.g. `openocd` / `st-flash`.)

## Status

Early development. Task creation, fake stack frame construction, and the kernel/port split are implemented. Context-switch triggering (`PendSV`), the scheduler's tick-driven decision logic, and blocking primitives (queues, mutexes, semaphores) are in progress.
