add_library(port_cortex_m4 STATIC 
    port.c
    port.s
)

target_include_directories(port_cortex_m4 PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/config 
)

target_link_libraries(port_cortex_m4 PUBLIC 
    kernel
)