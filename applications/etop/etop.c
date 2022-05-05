#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/tools/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include "eco_perf/metrics/cpu_usage.h"

#include "widgets/core_monitor.h"
#include "widgets/cpu_monitor.h"

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

    // Init the terminal widget containing the main widget
    terminal_application_t app;
    init_terminal_application(&app, main_widget);
    app.sleep_duration = n_seconds_sleep;

    // Main widget has horizontal layout
    twidget_linear_layout_t hlayout;
    twidget_linear_layout_init(&hlayout, CT_HORIZONTAL);
    hlayout.config.spacing = 3;
    twidget_set_layout(main_widget, &hlayout);

    // Each sub-widget has vertical layout
    tstack_t cpudata_stacks[3];
    twidget_linear_layout_t test_vlayouts[3];
    int cpubar_size = 35;
    for (int i = 0; i != 3; ++i)
    {
        twidget_linear_layout_init(&test_vlayouts[i], CT_VERTICAL);
        test_vlayouts[i].config.horizontal_align_mode = CT_TOP_OR_LEFT + i;
        test_vlayouts[i].config.vertical_align_mode = CT_TOP_OR_LEFT + i;
        tstack_init(&cpudata_stacks[i]);
        if (i == 1)
        {
            test_vlayouts[i].config.auto_children_resize = 0;
            cpudata_stacks[i].twidget.size.x = cpubar_size;
            cpudata_stacks[i].twidget.fixed_size.x = 1;
            test_vlayouts[i].config.spacing = 1;
        }
        twidget_set_layout(&cpudata_stacks[i].twidget, &test_vlayouts[i]);
        twidget_add_child(main_widget, &cpudata_stacks[i].twidget);
    }

    percent_bar_tstack_t *cpu_bars = malloc(3 * n_cpus * sizeof(percent_bar_tstack_t));
    for (int i = 0; i != 3 * first.n_cpus; ++i)
    {
        percent_bar_tstack_init(&cpu_bars[i]);
        cpu_bars[i].twidget.size.x = cpubar_size;
        cpu_bars[i].twidget.size.y = 1;
        twidget_add_child(
            &cpudata_stacks[i / n_cpus].twidget,
            &cpu_bars[i].twidget);
    }

    cpu_data_read(&last);
    cpu_data_diff(&first, &last, &diff);
    cpu_data_ratio(&diff, &ratio, n_seconds_sleep);

    // core_monitor_tstack_t core_monitor;
    // init_core_monitor_tstack(&core_monitor);
    // core_monitor.twidget.floating = 1;
    // core_monitor.twidget.pos.x = 120;
    // twidget_add_child(main_widget, &core_monitor.twidget);

    cpu_monitor_tstack_t cpu_monitor;
    init_cpu_monitor_tstack(&cpu_monitor);
    cpu_monitor.twidget.floating = 1;
    cpu_monitor.twidget.pos.x = 120;
    set_cpu_monitor_data(&cpu_monitor, &ratio);
    twidget_add_child(main_widget, &cpu_monitor.twidget);
    set_cpu_monitor_bounds(&cpu_monitor, -1, -1);

    unsigned char iter = 0;
    while (iter++ < max_iter)
    {
        cpu_data_read(&last);
        cpu_data_diff(&first, &last, &diff);
        cpu_data_ratio(&diff, &ratio, n_seconds_sleep);
        char title_buffer[15];
        sprintf(title_buffer, "core - %dx", iter);
        printf("etop - %d cores\n", first.n_cpus);
        for (int i = 0; i != 3 * n_cpus; ++i)
        {
            char cpu_name[50];
            sprintf(cpu_name, "CPU %d", i);
            update_cpu_data(&cpu_bars[i], &ratio.core_data[i % n_cpus]);
        }
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
    free(cpu_bars);
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
