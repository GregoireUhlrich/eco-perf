cmake_minimum_required(VERSION 3.16)

project(eco_perf C)

set(C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(eco_perf)
add_subdirectory(tests)
add_subdirectory(applications)
