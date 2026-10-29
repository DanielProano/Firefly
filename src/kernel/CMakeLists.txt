add_library(kernel STATIC 
    src/task.c
    src/scheduler.c
    src/mutex.c
    src/semaphore.c
    src/queue.c
)

target_link_libraries(kernel PUBLIC
    port_cortex_m4
)

target_link_libraries(kernel PRIVATE
    stm32_arch
    cmsis
)

target_include_directories(kernel PUBLIC 
    include
    ${PROJECT_SOURCE_DIR}/config
    ${PROJECT_SOURCE_DIR}/include
)