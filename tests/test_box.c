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
    box_tstack_t main_stack;
    init_box_tstack(&main_stack);
    init_terminal_application(&app, &main_stack.twidget);

    // Set layout for main widget. It should not resize the box,
    // only center it.
    twidget_linear_layout_t main_layout;
    init_twidget_linear_layout(&main_layout, CT_VERTICAL);
    set_twidget_layout(&main_stack.twidget, &main_layout);

    while (1)
    {
        update_terminal_application(&app);
    }
}
