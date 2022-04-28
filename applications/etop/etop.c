#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/tools/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include "eco_perf/metrics/cpu_usage.h"

#include "widgets/cpu_info.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void update_cpu_data(
    percent_bar_tmanager_t *percent_bar,
    cpu_core_data_t const *ratio)
{
    percent_bar_data_t *data = &percent_bar->data;
    init_percent_bar_data(data);
    add_percent_data(data, ratio->user_ratio, CT_GREEN);
    add_percent_data(data, ratio->sys_ratio, CT_BLUE);
    add_percent_data(data, ratio->nice_ratio, CT_YELLOW);
}

void display_cpu_data(unsigned short max_iter)
{
    double n_seconds_sleep = 1.5;

    cpu_data_t first, last, diff, ratio;

    // Initialize the data
    init_cpu_data(&first);
    init_cpu_data(&last);
    init_cpu_data(&diff);
    init_cpu_data(&ratio);

    read_cpu_data(&first);

    int n_cpus = first.n_cpus;

    // Main widget
    box_tmanager_t main_manager;
    init_box_tmanager(&main_manager);
    twidget_t *main_widget = &main_manager.twidget;

    // Init the terminal widget containing the main widget
    terminal_application_t app;
    init_terminal_application(&app, main_widget);
    app.sleep_duration = n_seconds_sleep;

    // Main widget has horizontal layout
    twidget_linear_layout_t hlayout;
    init_twidget_linear_layout(&hlayout, CT_HORIZONTAL);
    hlayout.config.spacing = 3;
    set_twidget_layout(main_widget, &hlayout);

    // Each sub-widget has vertical layout
    tmanager_t cpudata_managers[3];
    twidget_linear_layout_t test_vlayouts[3];
    int cpubar_size = 35;
    for (int i = 0; i != 3; ++i)
    {
        init_twidget_linear_layout(&test_vlayouts[i], CT_VERTICAL);
        test_vlayouts[i].config.horizontal_align_mode = CT_TOP_OR_LEFT + i;
        test_vlayouts[i].config.vertical_align_mode = CT_TOP_OR_LEFT + i;
        init_tmanager(&cpudata_managers[i]);
        if (i == 1)
        {
            test_vlayouts[i].config.auto_children_resize = 0;
            cpudata_managers[i].twidget.size.x = cpubar_size;
            cpudata_managers[i].twidget.fixed_size.x = 1;
            test_vlayouts[i].config.spacing = 1;
        }
        set_twidget_layout(&cpudata_managers[i].twidget, &test_vlayouts[i]);
        add_twidget_child(main_widget, &cpudata_managers[i].twidget);
    }

    percent_bar_tmanager_t *cpu_bars = malloc(3 * n_cpus * sizeof(percent_bar_tmanager_t));
    for (int i = 0; i != 3 * first.n_cpus; ++i)
    {
        init_percent_bar_tmanager(&cpu_bars[i]);
        cpu_bars[i].twidget.size.x = cpubar_size;
        cpu_bars[i].twidget.size.y = 1;
        add_twidget_child(
            &cpudata_managers[i / n_cpus].twidget,
            &cpu_bars[i].twidget);
    }

    read_cpu_data(&last);
    diff_cpu_data(&first, &last, &diff);
    calculate_ratio(&diff, &ratio, n_seconds_sleep);
    // cpu_info_twidget_t cpu_info;
    // init_cpu_info_twidget(&cpu_info, &ratio);
    // cpu_info.floating = 1;
    // add_twidget_child(&main_widget, &cpu_info);

    unsigned char iter = 0;
    while (iter++ < max_iter)
    {
        read_cpu_data(&last);
        diff_cpu_data(&first, &last, &diff);
        calculate_ratio(&diff, &ratio, n_seconds_sleep);
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
    free_cpu_data(&first);
    free_cpu_data(&last);
    free_cpu_data(&diff);
    free_cpu_data(&ratio);

    free(cpu_bars);
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
