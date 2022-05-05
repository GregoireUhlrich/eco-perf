#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/map.h"
#include "eco_perf/eco_std/vector.h"
#include "eco_perf/metrics/process.h"
#include "eco_perf/system/list_dir.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const bool verbose = true;

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
    list_directories("./");
    list_directories("/home");

    list_processes();

    es_container_t list;
    create_process_list(&list);

    es_vector_t list_view;
    es_vector_init(&list_view);

    es_map_t n_process_map;
    es_map_init(
        &n_process_map, 250, true,
        es_string_hash, es_string_eq);
    es_container_t counters;
    es_container_init(&counters, sizeof(int));
    es_container_reserve(&counters, 250);

    int iter = 0;
    int max_iter = 10;
    while (iter++ < max_iter)
    {
        float times[6];

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
        if (verbose)
        {
            for (int i = 0; i != list_view.size; ++i)
            {
                print_process_data_summary(list_view.data[i]);
            }
        }
        times[4] = (clock() - start) * 1000. / CLOCKS_PER_SEC;

        start = clock();
        for (int i = 0; i != list_view.size; ++i)
        {
            process_data_t *process = (process_data_t *)list_view.data[i];
            int *value = (int *)es_map_get(&n_process_map, &process->executable);
            if (value)
            {
                ++(*value);
            }
            else
            {
                int *value = malloc(sizeof(int));
                *value = 1;
                es_map_put(&n_process_map, &process->executable, value);
            }
        }
        times[5] = (clock() - start) * 1000. / CLOCKS_PER_SEC;
        if (verbose)
        {
            es_map_for_each(&n_process_map, print_map_item, NULL);
            puts("");
        }
        es_map_clear(&n_process_map);
        es_container_clear(&counters);

        if (verbose)
        {
            printf("%u processes in total\n", list_view.size);
            printf("Create           took %.2f milli-seconds\n", times[0]);
            printf("Copy             took %.2f milli-seconds\n", times[1]);
            printf("Sort (vector)    took %.2f milli-seconds\n", times[2]);
            printf("Sort (container) took %.2f milli-seconds\n", times[3]);
            printf("Print            took %.2f milli-seconds\n", times[4]);
            printf("Mapping          took %.2f milli-seconds\n", times[5]);
            printf("*********************************\n");
        }
        sleep(1);
    }

    es_map_free(&n_process_map);
    es_container_free(&counters);
    for (int i = 0; i != list_view.size; ++i)
    {
        free_process_data(list_view.data[i]);
    }
    es_container_free(&list);
    es_vector_free(&list_view);

    return 0;
}
