add_library(cute_terminal
terminal_application.h terminal_application.c)

target_link_libraries(cute_terminal
    io widgets terminal tools definitions)

add_subdirectory(io)
add_subdirectory(widgets)
add_subdirectory(terminal)
add_subdirectory(tools)
add_subdirectory(definitions)
