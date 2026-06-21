add_executable(blinky main.c ${CMAKE_SOURCE_DIR}/startup/startup_stm32f401.c)

target_link_libraries(blinky 
    hal_stm32f401
)

target_link_options(blinky PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/stm32f401.ld
)

