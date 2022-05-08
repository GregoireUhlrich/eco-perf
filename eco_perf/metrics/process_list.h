#ifndef ECO_PERF_PROCESS_LIST_H_INCLUDED
#define ECO_PERF_PROCESS_LIST_H_INCLUDED

#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/map.h"
#include "eco_perf/eco_std/vector.h"
#include "process.h"
typedef struct ProcessList
{
    es_container_t processes;
    es_map_t _dir_map;
    bool _clear_next;
} process_list_t;

void process_list_init(process_list_t *list);

void process_list_free(process_list_t *list);

void process_list_update(process_list_t *list);

void process_list_print(process_list_t *list);

#endif
