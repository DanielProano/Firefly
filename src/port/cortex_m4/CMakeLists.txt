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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../config
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)