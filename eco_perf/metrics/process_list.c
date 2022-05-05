#include "process_list.h"
#include "../system/file_stat.h"
#include "../system/list_dir.h"
#include "cpu_usage.h"
#include "eco_perf/cute_terminal/io/string_utils.h"
#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"
#include "memory_usage.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

void process_list_init(process_list_t *list)
{
    es_container_init(&list->_processes_data, sizeof(process_data_t));
    es_vector_init(&list->processes);
    es_map_init(&list->_metadata, 500, false, es_int_hash, es_int_eq);
}

void process_list_free(process_list_t *list)
{
    es_container_free(&list->_processes_data);
    es_vector_free(&list->processes);
    es_map_free(&list->_metadata);
}

static void _read_processes(process_list_t *list);

static void _update_processes(process_list_t *list);

void process_list_update(process_list_t *list)
{
    if (list->_processes_data.size == 0)
    {
        _read_processes(list);
    }
    else
    {
        _update_processes(list);
    }
}

void process_list_sort_view(
    process_list_t *list,
    es_comparator_t process_comp)
{
    es_qsort(
        es_vector_begin(&list->processes),
        es_vector_end(&list->processes),
        process_comp);
}

void process_list_print(process_list_t *list)
{
    for (int i = 0; i != list->processes.size; ++i)
    {
        print_process_data_summary(list->processes.data[i]);
    }
    printf("%u processes in total\n", list->processes.size);
}

int is_integer(char const *str)
{
    while (*str)
    {
        if (!isdigit(*str++))
        {
            return 0;
        }
    }
    return 1;
}

es_size_t _get_n_processes()
{
    es_size_t n_processes = 0;
    directory_lister_t lister;
    open_directory_lister(&lister, "/proc");
    char const *next;
    while ((next = get_next_directory(&lister)))
    {
        if (is_integer(next))
        {
            ++n_processes;
        }
    }
    close_directory_lister(&lister);
    if (n_processes < 0)
    {
        errno = EINVAL;
        perror("Invalid number of processes.");
        exit(1);
    }
    return n_processes;
}

void _read_processes(process_list_t *list)
{
    const es_size_t n_processes = _get_n_processes();
    es_container_free(&list->_processes_data);
    es_container_init(&list->_processes_data, sizeof(process_data_t));
    es_container_reserve(&list->_processes_data, 2 * n_processes);
    directory_lister_t lister;
    open_directory_lister(&lister, "/proc");
    char const *next;
    while ((next = get_next_directory(&lister)))
    {
        if (is_integer(next))
        {
            es_container_push(&list->_processes_data, ES_NULL);
            process_data_t *process = es_container_get(
                &list->_processes_data,
                list->_processes_data.size - 1);
            read_process_data(process, atoi(next));
            if (!process->valid)
            {
                --(list->_processes_data.size);
            }
        }
    }
    close_directory_lister(&lister);
}

void _update_processes(process_list_t *list)
{
    _read_processes(list);
}

void _read_process_stat_data(FILE *file, process_data_t *process);
void _read_process_statm_data(FILE *file, process_data_t *process);

void read_process_data(process_data_t *process, int dir)
{
    process->directory = dir;
    char process_file_name[50];
    sprintf(process_file_name, "/proc/%d/stat", dir);
    FILE *stat_file = fopen(process_file_name, "r");
    if (stat_file)
    {
        process->valid = 1;
        _read_process_stat_data(stat_file, process);
        fclose(stat_file);
    }
    else
    {
        process->valid = 0;
        return;
    }

    sprintf(process_file_name, "/proc/%d/statm", dir);
    stat_file = fopen(process_file_name, "r");
    if (stat_file)
    {
        _read_process_statm_data(stat_file, process);
        fclose(stat_file);
    }
    else
    {
        process->valid = 0;
        return;
    }
}

void _read_process_stat_data(FILE *file, process_data_t *process)
{
    int pid, ppid, pgrp, session, tty_nr, tpgid, exit_signal, processor, exit_code;
    long cutime, cstime, priority, nice, num_threads, itrealvalue, rss, cguest_time;
    unsigned int flags, rt_priority, policy;
    unsigned long minflt, cminflt, majflt, cmajflt, utime, stime, vsize, rsslim,
        startcode, endcode, startstack, kstkesp, kstseip, signal, blocked, sigignore,
        sigcatch, wchan, nswap, cnswap, guest_time, start_data, end_data, start_brk,
        end_vrk, arg_start, arg_end, env_start, env_end;
    unsigned long long starttime, delayacct_nlkio_ticks;
    char comm[NAME_MAX];
    char state;
    int n_scanned = fscanf(
        file,
        "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu "
        "%ld %ld %ld %ld %ld %ld %llu",
        &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr,
        &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt,
        &utime, &stime, &cutime, &cstime, &priority, &nice,
        &num_threads, &itrealvalue, &starttime);
    if (n_scanned < 22)
    {
        process->valid = 0;
        return;
    }
    process->pid = pid;
    char const *const first = comm + 1;
    const int len = strlen(comm) - 1;
    comm[len] = 0;
    es_string_assign_n(
        &process->executable,
        first,
        len);
    process->state = state;
    process->parent_pid = ppid;
    process->pgroup = pgrp;
    process->psession = session;
    process->flags = flags;
    process->priority = priority;
    process->nice_value = nice;
    process->num_threads = num_threads;
    process->start_time = starttime;
    const double ticks_per_sec = sysconf(_SC_CLK_TCK);
    process->cpu_usage.user_time = utime * 1. / ticks_per_sec;
    process->cpu_usage.sys_time = stime * 1. / ticks_per_sec;
    process->cpu_usage.nice_time = 0;

    n_scanned = fscanf(
        file,
        "%lu %ld %lu %lu %lu %lu %lu %lu ",
        &vsize, &rss, &rsslim, &startcode, &endcode, &startstack,
        &kstkesp, &kstseip);
    if (n_scanned < 8)
    {
        process->stack_pointer = 0;
        process->start_stack = 0;
        process->instruction_pointer = 0;
        return;
    }
    process->start_stack = startstack;
    process->stack_pointer = kstkesp;
    process->instruction_pointer = kstseip;

    n_scanned = fscanf(
        file,
        "%lu %lu %lu %lu %lu %lu %lu %d %d %u "
        "%u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
        &signal, &blocked, &sigignore, &sigcatch,
        &wchan, &nswap, &cnswap, &exit_signal, &processor,
        &rt_priority, &policy, &delayacct_nlkio_ticks,
        &guest_time, &cguest_time, &start_data, &end_data,
        &start_brk, &arg_start, &arg_end, &env_start, &env_end,
        &exit_code);
    if (n_scanned < 12)
    {
        process->accumulated_io_delay = 0;
    }
    else
    {
        process->accumulated_io_delay = delayacct_nlkio_ticks;
    }
}

void _read_process_statm_data(FILE *file, process_data_t *process)
{
    unsigned long size, resident, shared, text, lib, data, dt;
    fscanf(file, "%lu %lu %lu %lu %lu %lu %lu",
           &size, &resident, &shared, &text, &lib, &data, &dt);
    process->memory_usage.real = resident;
    process->memory_usage.virt = size - resident;
    process->memory_usage.shared = shared;
    process->text_memory = text;
    process->data_memory = text;
}
