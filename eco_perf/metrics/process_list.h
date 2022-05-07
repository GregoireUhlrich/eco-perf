#ifndef ECO_PERF_PROCESS_LIST_H_INCLUDED
#define ECO_PERF_PROCESS_LIST_H_INCLUDED

#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/map.h"
#include "eco_perf/eco_std/vector.h"
#include "process.h"

#define UNIX_PAGE_SIZE 4096
typedef struct ProcessList
{
    es_vector_t processes;
    es_container_t _processes_data;
    es_map_t _dir_map;
    bool _clear_next;
} process_list_t;

void process_list_init(process_list_t *list);

void process_list_free(process_list_t *list);

void process_list_update(process_list_t *list);

void process_list_sort_view(
    process_list_t *list,
    es_comparator_t process_comp);

void process_list_print(process_list_t *list);

#endif