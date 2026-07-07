# Firefly

Firefly is a hand-written preemptive, priority based real-time 
operating system with mutex, semaphore, and queue data structures 
implemented. It primarily targets the STM32F401CCU6 micro-controller
on the Cortex M architecture and was implemented in C & assembly.

## Motivation

I am developing an Autonomous Drone & Plane this summer! While 
I could just as easily use an open source real time operating 
system, the idea behind this project is to learn the 
foundation of embedded engineering & own the whole stack!

## Link

Read the full technical guide on my [website](https://dannyproano.com/research/firefly)!

## Build

Dependencies for Arch Linux:

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib cmake openocd
```
Then attach ST-Link to corresponding SWDIO, 
SDCLK, NRST, 3.3V, & GND on STM32.

From root, here is how to blink the light:

```bash
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build
./tools/flash.sh
```

To flash the Real-Time OS:

```bash
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build
./tools/flash.sh examples/rtos_demo/rtos_demo
```

## AI Policy

I believe AI is an exceptional tool but not a substitute for learning. 
Therefore, AI suggestions were considered, evaluated, and
applied to the file structure, some implementation details, and 
for general proof reading. However, NO code was generated was AI,
every line was typed & extensively thought about by me.