add_library(cmsis INTERFACE)

target_include_directories(cmsis SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/device
)
