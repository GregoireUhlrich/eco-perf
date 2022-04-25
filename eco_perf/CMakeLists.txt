add_library(eco_perf INTERFACE)

target_link_libraries(eco_perf INTERFACE metrics cute_terminal)

add_subdirectory(metrics)
add_subdirectory(cute_terminal)
