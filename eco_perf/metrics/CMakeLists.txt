add_library(metrics
    cpu_usage.h cpu_usage.c
    memory_usage.h memory_usage.c
    process.h process.c
    sort.h sort.c
)

target_link_libraries(metrics system)
