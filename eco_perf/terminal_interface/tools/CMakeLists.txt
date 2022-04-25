add_library(tools
    percent_bar.h percent_bar.c
)
target_link_libraries(tools io m)  # math library
