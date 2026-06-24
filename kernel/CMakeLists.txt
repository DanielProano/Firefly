add_library(kernel STATIC 
    src/task.c
)

target_link_libraries(kernel PUBLIC
    hal_stm32f401
)

target_include_directories(kernel PUBLIC 
    include
    ${CMAKE_SOURCE_DIR}/config
)