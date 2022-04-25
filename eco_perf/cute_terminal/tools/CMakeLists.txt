add_library(tools
    percent_bar.h percent_bar.c
    memory.h memory.c
    twidget_array.h twidget_array.c
)
target_link_libraries(tools io m)  # math library
