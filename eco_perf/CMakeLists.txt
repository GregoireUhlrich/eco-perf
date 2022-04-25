add_library(eco_perf INTERFACE)

target_link_libraries(eco_perf INTERFACE metrics terminal_interface)

add_subdirectory(metrics)
add_subdirectory(terminal_interface)
