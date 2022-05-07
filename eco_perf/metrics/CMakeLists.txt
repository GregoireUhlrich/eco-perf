add_library(metrics
    cpu_usage.h cpu_usage.c
    memory_usage.h memory_usage.c
    process.h process.c
    process_list.h process_list.c
)

target_link_libraries(metrics system)
