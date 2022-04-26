

add_executable(test_drawable test_drawable.c)
target_link_libraries(test_drawable eco_perf)

add_executable(test_terminal test_terminal.c)
target_link_libraries(test_terminal eco_perf)

add_executable(test_box test_box.c)
target_link_libraries(test_box eco_perf)

add_executable(answer_test_box answer_test_box.c)
target_link_libraries(answer_test_box eco_perf)
