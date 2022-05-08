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
#include <unistd.h>

void process_list_init(process_list_t *list)
{
    es_container_init(&list->processes, sizeof(process_data_t));
    es_map_init(&list->_dir_map, 500, false, es_int_hash, es_int_eq);
    list->_clear_next = false;
}

void process_list_free(process_list_t *list)
{
    es_container_free(&list->processes);
    es_map_free(&list->_dir_map);
}

static void _read_processes(process_list_t *list);

static void _update_processes(process_list_t *list);

void process_list_update(process_list_t *list)
{
    if (list->processes.size == 0 || list->_clear_next)
    {
        _read_processes(list);
        list->_clear_next = false;
    }
    else
    {
        _update_processes(list);
    }
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
    directory_lister_open(&lister, "/proc");
    char const *next;
    while ((next = directory_lister_next(&lister)))
    {
        if (is_integer(next))
        {
            ++n_processes;
        }
    }
    directory_lister_close(&lister);
    if (n_processes < 0)
    {
        errno = EINVAL;
        perror("Invalid number of processes.");
        exit(1);
    }
    return n_processes;
}

void _set_process_metadata(process_list_t *list)
{
    es_map_clear(&list->_dir_map);
    for (int i = 0; i != list->processes.size; ++i)
    {
        process_data_t *process = es_container_get(&list->processes, i);
        es_map_put(&list->_dir_map, &process->directory, process);
    }
}

void process_list_print(process_list_t *list)
{
    for (int i = 0; i != list->processes.size; ++i)
    {
        process_data_t *process = es_container_get(&list->processes, i);
        process_data_summary(process);
    }
}

void _read_processes(process_list_t *list)
{
    const es_size_t n_processes = _get_n_processes();
    es_container_free(&list->processes);
    es_container_init(&list->processes, sizeof(process_data_t));
    es_container_reserve(&list->processes, 2 * n_processes);
    directory_lister_t lister;
    directory_lister_open(&lister, "/proc");
    char const *next;
    while ((next = directory_lister_next(&lister)))
    {
        if (is_integer(next))
        {
            const int directory = atoi(next);
            es_container_push(&list->processes, ES_NULL);
            process_data_t *process = es_container_get(
                &list->processes,
                list->processes.size - 1);
            process_data_init(process);
            process_data_read(process, directory);
            if (!process->valid)
            {
                --(list->processes.size);
            }
        }
    }
    directory_lister_close(&lister);
    _set_process_metadata(list);
}

void _update_processes(process_list_t *list)
{
    directory_lister_t lister;
    directory_lister_open(&lister, "/proc");
    char const *next;
    es_size_t n_invalid = 0;
    while ((next = directory_lister_next(&lister)))
    {
        if (is_integer(next))
        {
            int directory = atoi(next);
            process_data_t *old_process = es_map_get(&list->_dir_map, &directory);
            if (old_process)
            {
                if (old_process->valid)
                {
                    process_data_read(old_process, directory);
                }
                else
                {
                    ++n_invalid;
                }
            }
            else
            {
                int realloc = es_container_push(&list->processes, ES_NULL);
                if (realloc)
                {
                    // Objects have been moved: start over
                    // This should be rare as the memory is never released
                    _read_processes(list);
                    directory_lister_close(&lister);
                    return;
                }
                process_data_t *process = es_container_get(
                    &list->processes,
                    list->processes.size - 1);
                process_data_init(process);
                process_data_read(process, directory);
                es_map_put(&list->_dir_map, &process->directory, process);
            }
        }
    }
    directory_lister_close(&lister);
    if (n_invalid > 0.3 * list->processes.size)
    {
        list->_clear_next = true;
    }
}

void _read_process_stat_data(FILE *file, process_data_t *process);

void process_data_read(process_data_t *process, int dir)
{
    process->directory = dir;
    char process_file_name[50];
    sprintf(process_file_name, "/proc/%d/stat", dir);
    FILE *stat_file = fopen(process_file_name, "r");
    process->valid = 1;
    bool no_previous = process->pid == -1;
    process->prev_cpu_usage = process->cpu_usage;
    process->prev_time = process->time;
    if (stat_file)
    {
        _read_process_stat_data(stat_file, process);
        if (no_previous)
        {
            process->prev_cpu_usage = process->cpu_usage;
            process->prev_time = process->time;
        }
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
    timespec_get(&process->time, TIME_UTC);
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
    fscanf(
        file,
        "%d %s %c",
        &pid, comm, &state);
    char *comm_ptr = comm;
    while (*comm_ptr)
    {
        if (*comm_ptr == '/')
        {
            *comm_ptr = '\0';
            break;
        }
        ++comm_ptr;
    }
    int n_scanned = fscanf(
        file,
        "%d %d %d %d %d %u %lu %lu %lu %lu %lu %lu "
        "%ld %ld %ld %ld %ld %ld %llu",
        &ppid, &pgrp, &session, &tty_nr,
        &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt,
        &utime, &stime, &cutime, &cstime, &priority, &nice,
        &num_threads, &itrealvalue, &starttime);
    if (n_scanned < 19)
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
    process->cpu_usage.user_time = utime * 1000. / ticks_per_sec;
    process->cpu_usage.sys_time = stime * 1000. / ticks_per_sec;
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
    process->memory_usage.real = rss * _SC_PAGE_SIZE / 1024; // in kB
    process->memory_usage.virt = vsize / 1024;               // in kB
    process->start_stack = startstack;
    process->stack_pointer = kstkesp;
    process->instruction_pointer = kstseip;

    // n_scanned = fscanf(
    //     file,
    //     "%lu %lu %lu %lu %lu %lu %lu %d %d %u "
    //     "%u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
    //     &signal, &blocked, &sigignore, &sigcatch,
    //     &wchan, &nswap, &cnswap, &exit_signal, &processor,
    //     &rt_priority, &policy, &delayacct_nlkio_ticks,
    //     &guest_time, &cguest_time, &start_data, &end_data,
    //     &start_brk, &arg_start, &arg_end, &env_start, &env_end,
    //     &exit_code);
}
