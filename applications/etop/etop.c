#include "eco_perf/cute_terminal/terminal_application.h"
#include "eco_perf/cute_terminal/tools/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include "eco_perf/metrics/cpu_usage.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void update_cpu_data(
    percent_bar_twidget_t *percent_bar,
    cpu_core_data_t const *ratio)
{
    percent_bar_data_t *data = (percent_bar_data_t *)(percent_bar->data);
    init_percent_bar_data(data);
    add_percent_data(data, ratio->user_ratio, CT_GREEN);
    add_percent_data(data, ratio->sys_ratio, CT_BLUE);
    add_percent_data(data, ratio->nice_ratio, CT_YELLOW);
}

void display_cpu_data()
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
    twidget_t main_widget;
    init_twidget(&main_widget);

    // Init the terminal widget containing the main widget
    terminal_application_t app;
    init_terminal_application(&app, &main_widget);
    app.sleep_duration = n_seconds_sleep;

    // Main widget has horizontal layout
    twidget_linear_layout_t hlayout;
    init_twidget_linear_layout(&hlayout, CT_HORIZONTAL);
    hlayout.config.spacing = 3;
    set_twidget_layout(&main_widget, &hlayout);

    // Each sub-widget has vertical layout
    twidget_t cpudata_widgets[3];
    twidget_linear_layout_t test_vlayouts[3];
    int cpubar_size = 35;
    for (int i = 0; i != 3; ++i)
    {
        init_twidget_linear_layout(&test_vlayouts[i], CT_VERTICAL);
        test_vlayouts[i].config.horizontal_align_mode = CT_TOP_OR_LEFT + i;
        init_twidget(&cpudata_widgets[i]);
        if (i == 1)
        {
            test_vlayouts[i].config.auto_children_resize = 0;
            cpudata_widgets[i].size.x = cpubar_size;
            cpudata_widgets[i].fixed_size.x = 1;
            test_vlayouts[i].config.spacing = 1;
        }
        set_twidget_layout(&cpudata_widgets[i], &test_vlayouts[i]);
        add_twidget_child(&main_widget, &cpudata_widgets[i]);
    }

    percent_bar_twidget_t *cpu_bars = malloc(3 * n_cpus * sizeof(percent_bar_twidget_t));
    percent_bar_config_t *cpu_configs = malloc(3 * n_cpus * sizeof(percent_bar_config_t));
    percent_bar_data_t *cpu_datas = malloc(3 * n_cpus * sizeof(percent_bar_data_t));
    for (int i = 0; i != 3 * first.n_cpus; ++i)
    {
        init_percent_bar_config(&cpu_configs[i]);
        init_percent_bar_data(&cpu_datas[i]);
        init_percent_bar_twidget(
            &cpu_bars[i],
            &cpu_datas[i],
            &cpu_configs[i]);
        cpu_bars[i].size.x = cpubar_size;
        cpu_bars[i].size.y = 1;
        add_twidget_child(&cpudata_widgets[i / n_cpus], &cpu_bars[i]);
    }

    while (1)
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
}

int main()
{
    display_cpu_data();
    return 0;
}
