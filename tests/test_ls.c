#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/vector.h"
#include "eco_perf/metrics/process.h"
#include "eco_perf/system/list_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

int main()
{
    list_directories("./");
    list_directories("/home");

    list_processes();

    es_container_t list;
    create_process_list(&list);

    es_vector_t list_view;
    es_vector_init(&list_view);

    int iter = 0;
    int max_iter = 10;
    while (iter++ < max_iter)
    {
        float times[5];

        time_t start = clock();
        update_process_list(&list);
        times[0] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        es_vector_resize(&list_view, list.size);
        for (int i = 0; i != list.size; ++i)
        {
            list_view.data[i] = es_container_get(&list, i);
        }
        times[1] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        es_vector_sort(&list_view, cpu_increasing);
        times[2] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        qsort(list.data, list.size, sizeof(process_data_t), cpu_increasing_int);
        times[3] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        for (int i = 0; i != list_view.size; ++i)
        {
            print_process_data_summary(list_view.data[i]);
        }
        times[4] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        printf("%u processes in total\n", list_view.size);
        printf("Create           took %.2f milli-seconds\n", times[0]);
        printf("Copy             took %.2f milli-seconds\n", times[1]);
        printf("Sort (vector)    took %.2f milli-seconds\n", times[2]);
        printf("Sort (container) took %.2f milli-seconds\n", times[3]);
        printf("Print            took %.2f milli-seconds\n", times[4]);
        printf("*********************************\n");
        sleep(1);
    }

    return 0;
}
