#include "program_list.h"
#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"

void program_data_init(program_data_t *program)
{
    es_string_init(&program->name);
    program->cpu_time = 0;
    program->memory = 0;
}

void program_data_free(program_data_t *program)
{
    es_string_free(&program->name);
}

static void _get_time(struct timespec *time)
{
    timespec_get(time, TIME_UTC);
}

void program_list_init(program_list_t *list)
{
    process_list_init(&list->processes);
    es_vector_init(&list->programs);
    es_container_init(&list->_programs, sizeof(program_data_t));
    es_container_reserve(&list->_programs, 500);
    es_map_init(&list->name_map, 500, false, es_string_hash, es_string_eq);
    _get_time(&list->last_record);
}

void program_list_free(program_list_t *list)
{
    for (int i = 0; i != list->_programs.size; ++i)
    {
        program_data_t *program = es_container_get(&list->_programs, i);
        program_data_free(program);
    }
    process_list_free(&list->processes);
    es_vector_free(&list->programs);
    es_container_free(&list->_programs);
    es_map_free(&list->name_map);
}

void _update_program_list(program_list_t *list, es_size_t n_sorted);

void program_list_update(program_list_t *list, es_size_t n_sorted)
{
    process_list_update(&list->processes);
    _update_program_list(list, n_sorted);
}

static void _set_sorted_view(program_list_t *list, es_size_t n_sorted);
static void _restore_map(program_list_t *list);

#include <stdio.h>
void _update_program_list(program_list_t *list, es_size_t n_sorted)
{
    struct timespec time;
    _get_time(&time);
    for (int i = 0; i != list->processes.processes.size; ++i)
    {
        process_data_t *process = es_container_get(&list->processes.processes, i);
        program_data_t *program = es_map_get(&list->name_map, &process->executable);
        if (!program)
        {
            bool alloc = es_container_push(&list->_programs, NULL);
            if (alloc)
            {
                _restore_map(list);
            }
            program = es_container_get(&list->_programs, list->_programs.size - 1);
            program_data_init(program);
            es_string_assign(&program->name, es_string_get(&process->executable));
            program->time_ref = time;
            program->time = time;
            es_map_put(&list->name_map, &program->name, program);
        }
        else
        {
            program->cpu_time += process->cpu_usage.nice_time - process->prev_cpu_usage.nice_time;
            program->cpu_time += process->cpu_usage.user_time - process->prev_cpu_usage.user_time;
            program->cpu_time += process->cpu_usage.sys_time - process->prev_cpu_usage.sys_time;
            int n_sec = time.tv_sec - program->time.tv_sec + 1e-9 * (time.tv_nsec - program->time.tv_nsec);
            program->memory += process->memory_usage.real * n_sec;
            program->time = time;
        }
    }
    list->last_record = time;
    _set_sorted_view(list, n_sorted);
}

bool _cpu_decreasing(es_cref_t prog1, es_cref_t prog2)
{
    return ((program_data_t *)prog1)->cpu_time > ((program_data_t *)prog2)->cpu_time;
}

void _set_sorted_view(program_list_t *list, es_size_t n_sorted)
{
    es_vector_clear(&list->programs);
    for (int i = 0; i != list->_programs.size; ++i)
    {
        program_data_t *program = es_container_get(&list->_programs, i);
        es_vector_push(&list->programs, program);
    }
    es_partial_qsort(
        es_vector_begin(&list->programs),
        es_vector_end(&list->programs),
        n_sorted,
        _cpu_decreasing);
}

void _restore_map(program_list_t *list)
{
    es_map_clear(&list->name_map);
    for (int i = 0; i != list->_programs.size; ++i)
    {
        program_data_t *program = es_container_get(&list->_programs, i);
        es_map_put(&list->name_map, &program->name, &program);
    }
}
