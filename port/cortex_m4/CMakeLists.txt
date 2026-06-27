add_library(port_cortex_m4 STATIC 
    src/port.c
    src/port.s
    src/nvic.c
    src/systick.c
)

target_link_libraries(port_cortex_m4 PUBLIC 
    kernel
)

target_include_directories(port_cortex_m4 PUBLIC 
    include
    ${CMAKE_SOURCE_DIR}/config 
)