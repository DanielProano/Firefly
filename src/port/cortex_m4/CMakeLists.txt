add_library(port_cortex_m4 STATIC 
    src/port.c
    src/port.s
    src/nvic.c
    src/systick.c
)

target_link_libraries(port_cortex_m4 PUBLIC
    stm32_arch
    hal_stm32f401
    cmsis
)


# Ensure it can find the CMSIS files
target_include_directories(port_cortex_m4 PUBLIC ${CMAKE_SOURCE_DIR}/external/cmsis/core)


target_include_directories(port_cortex_m4 PUBLIC
    include
    ${PROJECT_SOURCE_DIR}/config
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/kernel/include
)