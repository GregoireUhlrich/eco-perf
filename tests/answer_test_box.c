#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    // Init the application and the main widget
    terminal_application_t app;
    tstack_t main_stack;
    tstack_init(&main_stack);
    init_terminal_application(&app, &main_stack.twidget);
    app.sleep_duration = 0.5; // time between two iterations

    // Set layout for main widget
    twidget_linear_layout_t main_layout;
    twidget_linear_layout_init(&main_layout, CT_VERTICAL);
    twidget_set_layout(&main_stack.twidget, &main_layout);
    main_layout.config.auto_children_resize = 0;
    main_layout.config.vertical_align_mode = CT_CENTER;
    main_layout.config.horizontal_align_mode = CT_CENTER;

    // Init the box widget, with a fixed size
    // (the layout will not stretch it, only center it)
    box_tstack_t box;
    box_tstack_init(&box);
    terminal_vector_set(&box.twidget.size, 40, 10);
    twidget_add_child(&main_stack.twidget, &box.twidget);

    // Set the box layout: vertical layout (title and one percent bar)
    twidget_linear_layout_t box_layout;
    twidget_linear_layout_init(&box_layout, CT_VERTICAL);
    box_layout.config.spacing = 1; // Set one line spacing
    twidget_set_layout(&box.twidget, &box_layout);

    // Define title for the box
    text_line_tstack_t box_title;
    init_text_line_tstack(&box_title);
    twidget_add_child(&box.twidget, &box_title.twidget);

    // Define cpu_bar widget with dummy data (does not change)
    percent_bar_tstack_t cpu_bar;
    percent_bar_tstack_init(&cpu_bar);
    twidget_add_child(&box.twidget, &cpu_bar.twidget);
    percent_data_add(&cpu_bar.data, 0., CT_GREEN);

    const float n_seconds_sleep = 0.5;
    int index = 0;
    int color_index = 0;
    int n_colors = 5;
    int colors[] = {CT_GREEN, CT_BLUE, CT_RED, CT_CYAN, CT_MAGENTA};
    while (1)
    {
        if (index % 2 == 0)
        {
            text_line_set_content(&box_title, "Tu y arrivera aurelien!");
        }
        else
        {
            text_line_set_content(&box_title, "Ou pas ... :)))))))");
        }
        double percent = 20 * index / 100.;
        cpu_bar.data.data[0] = percent;
        cpu_bar.data.colors[0] = colors[color_index % n_colors];
        update_terminal_application(&app);
        ++index;
        if (percent >= 1)
        {
            index = 0;
            ++color_index;
        }
    }
    free_terminal_application(&app);
}
