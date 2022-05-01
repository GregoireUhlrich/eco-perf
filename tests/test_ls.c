#include "eco_perf/metrics/process.h"
#include "eco_perf/metrics/sort.h"
#include "eco_perf/system/list_dir.h"
#include <stdio.h>
#include <unistd.h>

int cpu_increasing(void const *lv, void const *rv)
{
    process_data_t const *l = (process_data_t const *)lv;
    process_data_t const *r = (process_data_t const *)rv;
    const double l_cpu = l->cpu_usage.user_time + l->cpu_usage.sys_time;
    const double r_cpu = r->cpu_usage.user_time + r->cpu_usage.sys_time;
    return l_cpu > r_cpu;
}

int main()
{
    list_directories("./");
    list_directories("/home");

    list_processes();

    size_t n_processes;
    process_data_t *list = create_process_list(&n_processes);

    int iter = 0;
    int max_iter = 10;
    while (iter++ < max_iter)
    {
        list = update_process_list(list, &n_processes);
        ep_sort(list, n_processes, sizeof(process_data_t), cpu_increasing);
        for (int i = 0; i != n_processes; ++i)
        {
            print_process_data_summary(&list[i]);
        }
        printf("%lu processes in total\n****************", n_processes);
        sleep(1);
    }

    return 0;
}
