# https://embedded.seas.upenn.edu/Guides/build-systems/cross-compilation-and-toolchain-files/
# Describes how & where to build

# Target OS, convention is generic for embedded
set(CMAKE_SYSTEM_NAME Generic)

# Name of processor type
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Runs per file
# mcpu: 
#   specify target CPU arch.
# mthumb:
#   M Cortex exclusively uses thumb: 
#   https://medium.com/@wadixtech/arm-cortex-m-thumb-encoding-fdb5f5c6b87b
#   thumb is a specialized instruction (16 bit) set for ARM
#   thumb2 uses 16 & 32 bit instructions
#   there does not exists a mthumb2 flag
# mfpu:
#   what FPU hardware is available
#   stm32f401 uses single-precision FPU, 16 double precision registers
#   without FPU, floating point calcs must be done with library functions
# mfloat:
#   https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
#   Three options:
#       soft: no FPU
#       softfp: FPU but passes floats in integer registers
#       hard: FPU & passes floats in FPU registers
# ffunction-sections:
#   gcc places each function in its own separate linker section
#   eliminates dead code but only alongside gc-sections
# fdata-sections:
#   same as ffunction but for global variables & constants

add_compile_options(
    -mcpu=cortex-m4
    -mthumb 
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffunction-sections
    -fdata-sections
)

# Same stuff as add compile but for linker
# specs=nosys.specs:
#   C lib has functions that need OS support
#   since we have no OS, disable it
# specs=nano.specs:
#   switch from full C lib to smaller binary
# -u _printf_float:
#   linker must include float printf implementation
#   newlib-nano by default strips out float support
# -Wl:
#   pass flag to linker
# --gc-sections:
#   garabage collect any sections with no references
add_link_options(
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nosys.specs
    -specs=nano.specs
    -u _printf_float
    -Wl,--gc-sections
)

# By default, CMake tries to compile a tiny test program
# This turns it off bc. it will fail ARM on my x86 laptop
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)