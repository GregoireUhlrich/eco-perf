add_library(widgets
    twidget.h twidget.c
    box.h box.c
    layouts.h layouts.c
    terminal.h terminal.c
    percent_bar.h percent_bar.c
)

target_link_libraries(widgets io)
