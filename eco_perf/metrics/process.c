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

void process_data_init(process_data_t *process)
{
    process->valid = 0;
    process->pid = -1;
    process->directory = -1;
    es_string_init(&process->executable);
    process->prev_cpu_usage.user_time = 0;
    process->prev_cpu_usage.sys_time = 0;
    process->prev_cpu_usage.nice_time = 0;
    process->memory_usage.real = 0;
    process->memory_usage.virt = 0;
    process->memory_usage.shared = 0;
    process->cpu_usage.user_time = 0;
    process->cpu_usage.sys_time = 0;
    process->cpu_usage.nice_time = 0;
}

void process_data_get_cmdline(
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

void process_data_summary(process_data_t *process)
{
    char real_mem[100], virt_mem[100];
    char command_line[4096];
    process_data_get_cmdline(command_line, process);
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

void process_data_free(process_data_t *process)
{
    es_string_free(&process->executable);
}
