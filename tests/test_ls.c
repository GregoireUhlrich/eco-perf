#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/map.h"
#include "eco_perf/eco_std/vector.h"
#include "eco_perf/metrics/process.h"
#include "eco_perf/metrics/process_list.h"
#include "eco_perf/system/list_dir.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const bool verbose = false;

bool cpu_increasing(es_cref_t lv, es_cref_t rv)
{
    process_data_t const *l = (process_data_t const *)lv;
    process_data_t const *r = (process_data_t const *)rv;
    const double l_cpu = l->cpu_usage.user_time + l->cpu_usage.sys_time;
    const double r_cpu = r->cpu_usage.user_time + r->cpu_usage.sys_time;
    return l_cpu < r_cpu;
}
int cpu_increasing_int(es_cref_t lv, es_cref_t rv)
{
    process_data_t const *l = (process_data_t const *)lv;
    process_data_t const *r = (process_data_t const *)rv;
    const double l_cpu = l->cpu_usage.user_time + l->cpu_usage.sys_time;
    const double r_cpu = r->cpu_usage.user_time + r->cpu_usage.sys_time;
    return l_cpu > r_cpu;
}

void print_map_item(es_ref_t key, es_ref_t value, es_ref_t data)
{
    printf("map[\"%s\"] = %d  ;", es_string_get(key), *(int const *)value);
}

int main()
{
    process_list_t processes;
    process_list_init(&processes);

    int iter = 0;
    int max_iter = 20;
    while (iter++ < max_iter)
    {
        float times[6];

        time_t start = clock();
        process_list_update(&processes);
        times[0] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        if (verbose)
        {
            process_list_print(&processes);
        }
        times[1] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        if (verbose)
        {
            printf("%u processes in total\n", processes.processes.size);
            printf("Create           took %.2f milli-seconds\n", times[0]);
            printf("Print            took %.2f milli-seconds\n", times[1]);
            printf("*********************************\n");
        }
        sleep(1);
    }

    process_list_free(&processes);

    return 0;
}
