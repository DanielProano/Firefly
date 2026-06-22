#!/bin/bash

set -e

TARGET="${1:-examples/blinky/blinky}"
ELF_PATH="build/${TARGET}"

if [ ! -f "$ELF_PATH" ]; then
    echo "Error: $ELF_PATH not found. Try building"
    exit 1
fi

openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32f4x.cfg -c "init" -c "reset halt" -c "program $ELF_PATH verify reset exit"
