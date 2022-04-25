add_library(graphics
    twidget.h twidget.c
    box.h box.c
    term_layouts.h term_layouts.c
    terminal.h terminal.c
    percent_bar.h percent_bar.c
)

target_link_libraries(graphics io)
