cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/stm32_toolchain.cmake)

project(Firefly C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(
    -Wall
    -Wextra
    -Werror
)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

add_library(stm32_arch INTERFACE)

target_compile_options(stm32_arch INTERFACE ${STM32_FLAGS} -Wall -Wextra -Werror)
target_link_options(stm32_arch INTERFACE ${STM32_LINK_FLAGS})

add_subdirectory(src/kernel)
add_subdirectory(src/port/cortex_m4)
add_subdirectory(src/hal/stm32f401)
add_subdirectory(external/cmsis)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Tests: ${BUILD_TESTS}")
message(STATUS "Examples: ${BUILD_EXAMPLES}")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "cmake . in root not allowed, use cmake -B build")
endif()