# https://embedded.seas.upenn.edu/Guides/build-systems/cross-compilation-and-toolchain-files/
# Describes how & where to build

set(CMAKE_SYSTEM_NAME Generic)  # Generic is convention for target OS

# Name of processor type
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(STM32_FLAGS
    -mcpu=cortex-m4         # specify target CPU arch.
    -mthumb                 # M Cortex only uses mthumb (16 & 32 bit) ARM instructions, https://medium.com/@wadixtech/arm-cortex-m-thumb-encoding-fdb5f5c6b87b
    -mfpu=fpv4-sp-d16       # enable single-precision FPU + 16 double precision registers
    -mfloat-abi=hard        # FPU enable & floats passed into FPU registers, https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html 
    -ffunction-sections     # GCC puts functions in separate linker section & eliminates dead code
    -fdata-sections         # GCC puts globals in separate linker section & eliminates dead code
)

set(STM32_LINK_FLAGS
    -mcpu=cortex-m4
    -mthumb                 # STM32F4 uses mthumb instructions
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nosys.specs      # Disable C lib functions that need OS support
    -specs=nano.specs       # Switch from full C lib to smaller binary
    -u _printf_float        # Tell linker to include float printf
    -Wl,--gc-sections       # -Wl: pass pag to linker, --gc-sections: garabage collect unreferenced sections
)

# By default, CMake tries to compile a tiny test program
# This turns it off bc. it will fail ARM on my x86 laptop
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)