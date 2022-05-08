#ifndef ECO_PERF_PROGRAM_LIST_H_INCLUDED
#define ECO_PERF_PROGRAM_LIST_H_INCLUDED

#include "process_list.h"
#include <time.h>

typedef struct ProgramData
{
    es_string_t name;

    struct timespec time_ref; // start recording time
    struct timespec time;     // last recorded time
    time_t cpu_time;          // in s
    es_size_t memory;         // in kB * s
} program_data_t;

typedef struct ProgramList
{
    es_vector_t programs;
    struct timespec last_record;
    process_list_t processes;
    es_container_t _programs;
    es_map_t name_map;
} program_list_t;

void program_data_init(program_data_t *program);

void program_data_free(program_data_t *program);

void program_list_init(program_list_t *list);

void program_list_free(program_list_t *list);

void program_list_update(program_list_t *list, es_size_t n_sorted);

#endif
