#include "eco_perf/cross_os_std.h"
#include "eco_perf/graphics/percent_bar.h"
#include "eco_perf/io/io.h"
#include "eco_perf/io/terminal_cursor.h"
#include "eco_perf/metrics/cpu_usage.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void display_ratio(cpu_core_data_t const *ratio, char const *cpu_name)
{
    char buffer[500];
    apply_format(buffer, cpu_name, BOLD);
    printf("%s - ", buffer);
    percent_bar_config_t config;
    load_default_bar_config(&config);
    percent_bar_data_t data;
    init_percent_bar_data(&data);
    add_percent_data(&data, ratio->user_ratio, GREEN);
    add_percent_data(&data, ratio->sys_ratio, BLUE);
    add_percent_data(&data, ratio->nice_ratio, YELLOW);
    create_percent_bar(buffer, &data, &config);
    printf("%s\n", buffer);
}

void display_cpu_data()
{
    clear_terminal();
    double n_seconds_sleep = 1.5;

    cpu_data_t first, last, diff, ratio;

    // Initialize the data
    init_cpu_data(&first);
    init_cpu_data(&last);
    init_cpu_data(&diff);
    init_cpu_data(&ratio);

    read_cpu_data(&first);

    while (1)
    {
        read_cpu_data(&last);
        diff_cpu_data(&first, &last, &diff);
        calculate_ratio(&diff, &ratio, n_seconds_sleep);
        printf("etop - %d cores\n", first.n_cpus);
        for (int i = 0; i != diff.n_cpus; ++i)
        {
            char cpu_name[50];
            sprintf(cpu_name, "CPU %d", i);
            display_ratio(&ratio.core_data[i], cpu_name);
        }
        usleep(n_seconds_sleep * 1e6);
        clear_terminal();

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
