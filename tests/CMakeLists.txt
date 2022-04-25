

add_executable(test_drawable test_drawable.c)
target_link_libraries(test_drawable eco_perf)

add_executable(test_terminal test_terminal.c)
target_link_libraries(test_terminal eco_perf)
