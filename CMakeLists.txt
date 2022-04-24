cmake_minimum_required(VERSION 3.16)

project(eco_perf C)

set(C_STANDARD 11)

add_executable(etop etop.c)
target_link_libraries(etop eco_perf m)

add_executable(test_terminal test_terminal.c)
target_link_libraries(test_terminal eco_perf)

add_executable(test_drawable test_drawable.c)
target_link_libraries(test_drawable eco_perf)

add_subdirectory(eco_perf)
