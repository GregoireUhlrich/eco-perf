add_library(eco_perf
    cross_os_std.h
)
set_target_properties(eco_perf PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(eco_perf metrics io graphics)

add_subdirectory(metrics)
add_subdirectory(io)
add_subdirectory(graphics)
