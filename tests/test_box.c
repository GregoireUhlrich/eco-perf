#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    /*
    Exercise: application with a box centered in the terminal, containing
    a title and a percent bar (with dummy data).

    Hints:
     - For each object created (widget, layout, data, config) the
     corresponding initialization function MUST be called.
     - Each time a new layout or widget is created, it should be associated
     EXPLICITELY with the corresponding parent widget.
     - Start with the simple box without layout, the percent_bar will be
     automatically added. From there, add the other elements and the layouts.
     This step, including the definition of the box size is 6 lines.
     - Look at the initialization functions for each type and deduce the code
     to write from there.
    */

    // Init the application and the main widget
    terminal_application_t app;
    twidget_t main_widget;
    // ...

    // Set layout for main widget. It should not resize the box,
    // only center it.
    twidget_linear_layout_t main_layout;
    // ...

    // Init the box widget: set a (40, 5) size
    box_twidget_t box;
    // ...

    // Set the box layout: vertical layout with spacing 1
    twidget_linear_layout_t box_layout;
    // ...

    // Define the title widget for the box. Fix the title vertical size to 1
    text_line_twidget_t box_title;
    text_line_twidget_data_t title_data;
    // ...

    // Define cpu_bar widget with dummy data (does not change)
    // The code below is given but will not work until proper
    // initialization is set for the parent objects :)
    percent_bar_twidget_t cpu_bar;
    percent_bar_data_t cpu_data;
    init_percent_bar_data(&cpu_data);
    add_percent_data(&cpu_data, 0., CT_GREEN);
    percent_bar_config_t cpu_config;
    init_percent_bar_config(&cpu_config);
    init_percent_bar_twidget(&cpu_bar, &cpu_data, &cpu_config);
    cpu_bar.size.x = 20;
    cpu_bar.size.y = 1;
    add_twidget_child(&box, &cpu_bar);

    // Code given: make the percent bar update at each iteration
    int index = 0;
    int color_index = 0;
    int n_colors = 5;
    int colors[] = {CT_GREEN, CT_BLUE, CT_RED, CT_CYAN, CT_MAGENTA};
    while (1)
    {
        // Code given: update the percent bar
        double percent = 20 * index / 100.;
        cpu_data.data[0] = percent;
        cpu_data.colors[0] = colors[color_index % n_colors];
        ++index;
        if (percent >= 1)
        {
            index = 0;
            ++color_index;
        }

        // Final call to update the terminal
        update_terminal_application(&app);
    }
}
