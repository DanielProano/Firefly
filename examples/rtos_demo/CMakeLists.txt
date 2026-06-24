add_executable(rtos_demo main.c ${CMAKE_SOURCE_DIR}/startup/startup_stm32f401.c)

target_link_libraries(rtos_demo
    hal_stm32f401
)

target_link_options(rtos_demo PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/stm32f401.ld
)

