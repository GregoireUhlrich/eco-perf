add_executable(etop etop.c)
target_link_libraries(etop etop_widgets eco_perf)

add_executable(test test.c)
target_link_libraries(test etop_widgets eco_perf)

add_subdirectory(widgets)
