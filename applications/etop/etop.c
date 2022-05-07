#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/metrics/cpu_usage.h"

#include "widgets/cpu_monitor.h"
#include "widgets/proc_monitor.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void update_cpu_data(
    percent_bar_tstack_t *percent_bar,
    cpu_core_data_t const *ratio)
{
    percent_bar_data_t *data = &percent_bar->data;
    percent_bar_data_init(data);
    percent_data_add(data, ratio->user_ratio, CT_GREEN);
    percent_data_add(data, ratio->sys_ratio, CT_BLUE);
    percent_data_add(data, ratio->nice_ratio, CT_YELLOW);
}

void display_cpu_data(unsigned short max_iter)
{
    double n_seconds_sleep = 1.5;

    cpu_data_t first, last, diff, ratio;

    // Initialize the data
    cpu_data_init(&first);
    cpu_data_init(&last);
    cpu_data_init(&diff);
    cpu_data_init(&ratio);

    cpu_data_read(&first);

    int n_cpus = first.n_cpus;

    // Main widget
    box_tstack_t main_stack;
    box_tstack_init(&main_stack);
    twidget_t *main_widget = &main_stack.twidget;
    twidget_linear_layout_t layout;
    twidget_linear_layout_init(&layout, CT_VERTICAL);
    twidget_set_layout(main_widget, &layout);

    // Init the terminal widget containing the main widget
    terminal_application_t app;
    init_terminal_application(&app, main_widget);
    app.sleep_duration = n_seconds_sleep;

    cpu_data_read(&last);
    cpu_data_diff(&first, &last, &diff);
    cpu_data_ratio(&diff, &ratio, n_seconds_sleep);

    cpu_monitor_tstack_t cpu_monitor;
    init_cpu_monitor_tstack(&cpu_monitor);
    set_cpu_monitor_data(&cpu_monitor, &ratio);
    twidget_add_child(main_widget, &cpu_monitor.twidget);

    proc_monitor_tstack_t proc_monitor;
    proc_monitor_tstack_init(&proc_monitor);
    twidget_add_child(main_widget, &proc_monitor.twidget);

    unsigned char iter = 0;
    while (iter++ < max_iter)
    {
        cpu_data_read(&last);
        cpu_data_diff(&first, &last, &diff);
        cpu_data_ratio(&diff, &ratio, n_seconds_sleep);
        update_terminal_application(&app);

        // Swap first and last data for next iteration
        cpu_core_data_t *foo = first.core_data;
        first.core_data = last.core_data;
        last.core_data = foo;
    }

    // Free all memory
    cpu_data_free(&first);
    cpu_data_free(&last);
    cpu_data_free(&diff);
    cpu_data_free(&ratio);

    free_terminal_application(&app);
}

int main(int argc, char const *argv[])
{
    unsigned short max_iter = -1;
    if (argc > 1)
    {
        max_iter = atoi(argv[1]);
    }
    display_cpu_data(max_iter);
    return 0;
}
