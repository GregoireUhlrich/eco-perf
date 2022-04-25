add_library(cute_terminal INTERFACE)

target_link_libraries(cute_terminal INTERFACE
    io widgets terminal tools)

add_subdirectory(io)
add_subdirectory(widgets)
add_subdirectory(terminal)
add_subdirectory(tools)
