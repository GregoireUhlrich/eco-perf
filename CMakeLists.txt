cmake_minimum_required(VERSION 3.16)

project(eco_perf C)

set(C_STANDARD 11)

add_executable(main main.c)
target_link_libraries(main eco_perf)

add_subdirectory(eco_perf)
