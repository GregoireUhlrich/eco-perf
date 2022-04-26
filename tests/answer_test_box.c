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
    twidget_t main_widget;
    init_twidget(&main_widget);
    init_terminal_application(&app, &main_widget);
    app.sleep_duration = 0.5; // time between two iterations

    // Set layout for main widget
    twidget_linear_layout_t main_layout;
    init_twidget_linear_layout(&main_layout, CT_VERTICAL);
    set_twidget_layout(&main_widget, &main_layout);
    main_layout.config.auto_children_resize = 0;
    main_layout.config.vertical_align_mode = CT_CENTER;
    main_layout.config.horizontal_align_mode = CT_CENTER;

    // Init the box widget, with a fixed size
    // (the layout will not stretch it, only center it)
    box_twidget_t box;
    init_box_twidget(&box);
    set_terminal_vector(&box.size, 40, 5);
    add_twidget_child(&main_widget, &box);

    // Set the box layout: vertical layout (title and one percent bar)
    twidget_linear_layout_t box_layout;
    init_twidget_linear_layout(&box_layout, CT_VERTICAL);
    box_layout.config.spacing = 1; // Set one line spacing
    set_twidget_layout(&box, &box_layout);

    // Define title for the box
    text_line_twidget_t box_title;
    text_line_twidget_data_t title_data;
    init_text_line_twidget_data(&title_data);
    init_text_line_twidget(&box_title, &title_data);
    box_title.fixed_size.x = 1;
    add_twidget_child(&box, &box_title);

    // Define cpu_bar widget with dummy data (does not change)
    percent_bar_twidget_t cpu_bar;
    percent_bar_data_t cpu_data;
    init_percent_bar_data(&cpu_data);
    add_percent_data(&cpu_data, 0., CT_GREEN);
    percent_bar_config_t cpu_config;
    init_percent_bar_config(&cpu_config);
    init_percent_bar_twidget(&cpu_bar, &cpu_data, &cpu_config);
    add_twidget_child(&box, &cpu_bar);

    const float n_seconds_sleep = 0.5;
    int index = 0;
    int color_index = 0;
    int n_colors = 5;
    int colors[] = {CT_GREEN, CT_BLUE, CT_RED, CT_CYAN, CT_MAGENTA};
    while (1)
    {
        if (index % 2 == 0)
        {
            set_text_line_twidget_data(&box_title, "Tu y arrivera aurelien!");
        }
        else
        {
            set_text_line_twidget_data(&box_title, "Ou pas ... :)))))))");
        }
        double percent = 20 * index / 100.;
        cpu_data.data[0] = percent;
        cpu_data.colors[0] = colors[color_index % n_colors];
        update_terminal_application(&app);
        ++index;
        if (percent >= 1)
        {
            index = 0;
            ++color_index;
        }
    }
}
