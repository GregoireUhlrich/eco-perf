#include "program_list.h"
#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"

void program_data_init(program_data_t *program)
{
    es_string_init(&program->name);
    program->cpu_time = 0;
    program->memory = 0;
    program->begin_time_ms = 0;
    program->current_time_ms = 0;
}

void program_data_free(program_data_t *program)
{
    es_string_free(&program->name);
}

void program_list_init(program_list_t *list)
{
    process_list_init(&list->processes);
    es_vector_init(&list->programs);
    es_container_init(&list->_programs, sizeof(program_data_t));
    es_container_reserve(&list->_programs, 500);
    es_map_init(&list->name_map, 500, false, es_string_hash, es_string_eq);
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

void _update_program_list(program_list_t *list, es_size_t n_sorted)
{
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
            program->begin_time_ms = 1e3 * (process->time.tv_sec + 1e-9 * process->time.tv_nsec);
            program->current_time_ms = program->begin_time_ms;
            es_string_assign(&program->name, es_string_get(&process->executable));
            es_map_put(&list->name_map, &program->name, program);
        }
        else
        {
            cpu_core_data_t cpu_time = process_data_get_cpu_diff(process);
            memory_data_t mem = process_data_get_cumulative_memory_s(process);
            program->cpu_time += cpu_time.nice_time;
            program->cpu_time += cpu_time.user_time;
            program->cpu_time += cpu_time.sys_time;
            program->memory += mem.real;
            program->current_time_ms = 1e3 * (process->time.tv_sec + 1e-9 * process->time.tv_nsec);
        }
    }
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
