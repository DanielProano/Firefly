# STATIC forces everything into archive (.a) file
add_library(hal_stm32f401 STATIC 
    src/rcc.c
    src/gpio.c
    src/fpu.c
    src/fault_indicator.c
)

# options for visibility:
#   PUBLIC: everything can see it
#   PRIVATE: only this hal can see it
#   INTERFACE: only thing linked against it explicitly can see it
#
# Translates to a -I flag passed to GCC, meaning search this directory for headers
target_include_directories(hal_stm32f401 PUBLIC 
    include 
    ${CMAKE_SOURCE_DIR}/config 
)

target_link_libraries(hal_stm32f401 PUBLIC 
    port_cortex_m4
)