add_library(widgets
    twidget.h twidget.c
    twidget_interface.h twidget_interface.c
    box.h box.c
    list.h list.c
    terminal.h terminal.c
    percent_bar.h percent_bar.c
    text_line.h text_line.c
)

target_link_libraries(widgets layouts io)

add_subdirectory(layouts)
