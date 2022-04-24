add_library(graphics
    percent_bar.h percent_bar.c
    term_drawable.h term_drawable.c
    term_box.h term_box.c
)

target_link_libraries(graphics io m)
