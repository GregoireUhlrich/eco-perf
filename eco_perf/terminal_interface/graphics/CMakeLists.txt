add_library(graphics
    twidget.h twidget.c
    term_box.h term_box.c
    term_layouts.h term_layouts.c
    terminal_twidget.h terminal_twidget.c
    drawable_percent_bar.h drawable_percent_bar.c
)

target_link_libraries(graphics io)
