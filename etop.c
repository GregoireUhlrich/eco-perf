#include "eco_perf/cross_os_std.h"
#include "eco_perf/io/io.h"
#include "eco_perf/io/terminal_cursor.h"
#include "eco_perf/metrics/cpu_usage.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int bar_size = 32;
char left_char = '[';
char right_char = ']';
char fill_char = ' ';
char usage_char = '|';

char *fill_str(char *destination, char filler, int n_repeat)
{
    for (int i = 0; i != n_repeat; ++i)
    {
        destination[i] = filler;
    }
    destination[n_repeat] = '\0';
    return destination;
}

void display_ratio(cpu_core_data_t const *ratio, char const *cpu_name)
{
    const int n_user_char = ceil(bar_size * ratio->user_ratio);
    const int n_sys_char = ceil(bar_size * ratio->sys_ratio);
    const int remaining_chars = bar_size - n_user_char - n_sys_char;

    char buffer[500];
    if (remaining_chars < bar_size / 3)
    {
        printf("%s - ", apply_foreground_color(buffer, cpu_name, RED));
    }
    else if (remaining_chars > 2 * bar_size / 3)
    {
        printf("%s - ", apply_foreground_color(buffer, cpu_name, GREEN));
    }
    else
    {
        printf("%s - ", apply_foreground_color(buffer, cpu_name, BLUE));
    }

    apply_format(buffer, "[", BOLD);
    printf("%s", buffer);
    char user_buffer[100];
    char sys_buffer[100];
    apply_foreground_color(buffer, fill_str(user_buffer, usage_char, n_user_char), GREEN);
    printf("%s", buffer);
    apply_foreground_color(buffer, fill_str(sys_buffer, usage_char, n_sys_char), BLUE);
    printf("%s", buffer);
    printf("%s", fill_str(buffer, fill_char, remaining_chars));
    apply_format(buffer, "]", BOLD);
    printf("%s %.1f%%\n", buffer, 100 * (ratio->user_ratio + ratio->sys_ratio));
}

int main()
{
    double n_seconds_sleep = 1.5;

    cpu_data_t first, last, diff, ratio;

    // Initialize the data
    init_cpu_data(&first);
    init_cpu_data(&last);
    init_cpu_data(&diff);
    init_cpu_data(&ratio);

    read_cpu_data(&first);
    printf("etop - %d cores\n", first.n_cpus);

    while (1)
    {
        read_cpu_data(&last);
        diff_cpu_data(&first, &last, &diff);
        calculate_ratio(&diff, &ratio, n_seconds_sleep);
        for (int i = 0; i != diff.n_cpus; ++i)
        {
            char cpu_name[50];
            sprintf(cpu_name, "CPU %d", i);
            display_ratio(&ratio.core_data[i], cpu_name);
        }
        usleep(n_seconds_sleep * 1e6);
        clear_lines_up(4);

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

    return 0;
}
