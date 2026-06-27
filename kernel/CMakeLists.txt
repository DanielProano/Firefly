add_library(kernel STATIC 
    src/task.c
    src/scheduler.c
    src/mutex.c
    src/semaphore.c
    src/queue.c
)

target_link_libraries(kernel PUBLIC
    hal_stm32f401
)

target_include_directories(kernel PUBLIC 
    include
    ${CMAKE_SOURCE_DIR}/config
)