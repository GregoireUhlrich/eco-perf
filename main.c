#include "eco_perf/cross_os_std.h"
#include "eco_perf/metrics/cpu_usage.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
    int n_seconds_sleep = 3;

    cpu_data_t first;
    read_cpu_data(&first);
    print_cpu_data(&first);

    sleep(n_seconds_sleep);

    cpu_data_t last;
    read_cpu_data(&last);
    print_cpu_data(&last);

    cpu_data_t diff;
    diff_cpu_data(&first, &last, &diff);
    printf("Diff for sleep time of %d s\n", n_seconds_sleep);
    print_cpu_data(&diff);

    free_cpu_data(&first);
    free_cpu_data(&last);
    free_cpu_data(&diff);
}
