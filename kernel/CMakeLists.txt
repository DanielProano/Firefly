add_library(kernel STATIC 
    src/task.c
)

target_include_directories(kernel PUBLIC 
    include
    ${CMAKE_SOURCE_DIR}/config
)