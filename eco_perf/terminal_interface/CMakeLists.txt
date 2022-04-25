add_library(terminal_interface INTERFACE)

target_link_libraries(terminal_interface INTERFACE
    io graphics terminal tools)

add_subdirectory(io)
add_subdirectory(graphics)
add_subdirectory(terminal)
add_subdirectory(tools)
