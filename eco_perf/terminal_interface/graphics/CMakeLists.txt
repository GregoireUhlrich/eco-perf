add_library(graphics
    percent_bar.h percent_bar.c
    term_drawable.h term_drawable.c
    term_box.h term_box.c
    term_layouts.h term_layouts.c
    terminal_panel.h terminal_panel.c
    drawable_percent_bar.h drawable_percent_bar.c
)

target_link_libraries(graphics io m)
