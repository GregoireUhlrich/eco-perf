#include "process.h"
#include "../system/list_dir.h"
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

void init_process_data(process_data_t *process)
{
    process->valid = 0;
    process->pid = -1;
    process->directory = -1;
    es_string_init(&process->executable);
    process->memory_usage.real = 0;
    process->memory_usage.virt = 0;
    process->memory_usage.shared = 0;
    process->cpu_usage.user_time = 0;
    process->cpu_usage.sys_time = 0;
    process->cpu_usage.nice_time = 0;
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

void get_process_command_line(
    char *destination,
    process_data_t *process)
{
    char process_file_name[60];
    sprintf(process_file_name, "/proc/%d/cmdline", process->directory);
    FILE *file = fopen(process_file_name, "r");
    *destination = '\0';
    if (file)
    {
        fscanf(file, "%s", destination);
        fclose(file);
    }
    else
    {
        return;
    }
}

void print_process_data_summary(process_data_t *process)
{
    char real_mem[100], virt_mem[100];
    char command_line[4096];
    get_process_command_line(command_line, process);
    print_nice_memory(real_mem, process->memory_usage.real);
    print_nice_memory(virt_mem, process->memory_usage.virt);
    printf(
        "Process '%s' (pid=%d, ppid=%d): cpu_time (user: %.3fs, sys: %.3fs), "
        "mem (real: %s, virt: %s) -- [%s]\n",
        es_string_get(&process->executable),
        process->pid, process->parent_pid,
        process->cpu_usage.user_time, process->cpu_usage.sys_time,
        real_mem, virt_mem,
        command_line);
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

void list_processes()
{
    directory_lister_t lister;
    open_directory_lister(&lister, "/proc");
    char const *next;
    while ((next = get_next_directory(&lister)))
    {
        if (is_integer(next))
        {
            // process directory
            process_data_t process;
            init_process_data(&process);
            read_process_data(&process, atoi(next));
            if (process.valid)
            {
                print_process_data_summary(&process);
            }
            free_process_data(&process);
        }
    }
    close_directory_lister(&lister);
}

void create_process_list(es_container_t *container)
{
    directory_lister_t lister;
    open_directory_lister(&lister, "/proc");
    char const *next;
    int n_processes = 0;
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

    es_container_init(container, sizeof(process_data_t));
    es_container_reserve(container, 2 * n_processes);
    update_process_list(container);
}

void update_process_list(es_container_t *container)
{
    directory_lister_t lister;
    open_directory_lister(&lister, "/proc");
    char const *next;
    for (int i = 0; i != container->size; ++i)
    {
        free_process_data(es_container_get(container, i));
    }
    es_container_clear(container);
    while ((next = get_next_directory(&lister)))
    {
        if (is_integer(next))
        {
            es_container_push(container, ES_NULL);
            read_process_data(
                es_container_get(container, container->size - 1),
                atoi(next));
        }
    }
    close_directory_lister(&lister);
}

void free_process_list(es_container_t *container)
{
    es_container_free(container);
}

void free_process_data(process_data_t *process)
{
    es_string_free(&process->executable);
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
