#include "eco_perf/cross_os_std.h"
#include "eco_perf/metrics/cpu_usage.h"
#include "eco_perf/terminal_interface/io/io.h"
#include "eco_perf/terminal_interface/terminal/cursor.h"
#include "eco_perf/terminal_interface/tools/percent_bar.h"
#include "eco_perf/terminal_interface/widgets/layouts.h"
#include "eco_perf/terminal_interface/widgets/percent_bar.h"
#include "eco_perf/terminal_interface/widgets/terminal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void update_cpu_data(
    percent_bar_widget_t *percent_bar,
    cpu_core_data_t const *ratio)
{
    percent_bar_data_t *data = (percent_bar_data_t *)(percent_bar->data);
    init_percent_bar_data(data);
    add_percent_data(data, ratio->user_ratio, GREEN);
    add_percent_data(data, ratio->sys_ratio, BLUE);
    add_percent_data(data, ratio->nice_ratio, YELLOW);
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

    terminal_twidget_t terminal;
    hlayout_twidget_t main_widget;
    layout_twidget_config_t default_layout_config = get_default_term_layout_config();
    init_hlayout_twidget(&main_widget, &default_layout_config);
    init_terminal_twidget(&terminal, &main_widget);

    vlayout_twidget_t test_vlayouts[3];
    layout_twidget_config_t vlayouts_configs[3];
    for (int i = 0; i != 3; ++i)
    {
        vlayouts_configs[i] = get_default_term_layout_config();
        vlayouts_configs[i].align_mode = TERM_LAYOUT_TOPLEFT + i;
        if (i == 1)
        {
            vlayouts_configs[i].auto_children_resize = 0;
        }
        init_vlayout_twidget(&test_vlayouts[i], &vlayouts_configs[i]);
        add_twidget_child(&main_widget, &test_vlayouts[i]);
    }

    percent_bar_widget_t *cpu_bars = malloc(3 * n_cpus * sizeof(percent_bar_widget_t));
    percent_bar_config_t *cpu_configs = malloc(3 * n_cpus * sizeof(percent_bar_config_t));
    percent_bar_data_t *cpu_datas = malloc(3 * n_cpus * sizeof(percent_bar_data_t));
    for (int i = 0; i != 3 * first.n_cpus; ++i)
    {
        load_default_bar_config(&cpu_configs[i]);
        init_percent_bar_data(&cpu_datas[i]);
        init_percent_bar_twidget(
            &cpu_bars[i],
            &cpu_datas[i],
            &cpu_configs[i]);
        cpu_bars[i].size.x = 40;
        cpu_bars[i].size.y = 1;
        add_twidget_child(&test_vlayouts[i / n_cpus], &cpu_bars[i]);
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
        update_twidget(&terminal);
        draw_twidget(&terminal);
        fflush(stdout);
        usleep(n_seconds_sleep * 1e6);

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
