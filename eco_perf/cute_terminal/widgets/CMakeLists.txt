add_library(widgets
    twidget.h twidget.c
    twidget_interface.h
    box.h box.c
    terminal.h terminal.c
    percent_bar.h percent_bar.c
    text_line.h text_line.c
)

target_link_libraries(widgets layouts io)

add_subdirectory(layouts)
