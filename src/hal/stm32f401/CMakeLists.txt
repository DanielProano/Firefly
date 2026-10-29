# STATIC forces everything into archive (.a) file
add_library(hal_stm32f401 STATIC 
    src/rcc.c
    src/gpio.c
    src/fpu.c
    src/fault_indicator.c
)

target_link_libraries(hal_stm32f401 PUBLIC
    stm32_arch
    cmsis
)

# options for visibility:
#   PUBLIC: everything can see it
#   PRIVATE: only this hal can see it
#   INTERFACE: only thing linked against it explicitly can see it
#
# Translates to a -I flag passed to GCC, meaning search this directory for headers
target_include_directories(hal_stm32f401 PUBLIC 
    include 
    ${PROJECT_SOURCE_DIR}/config 
)