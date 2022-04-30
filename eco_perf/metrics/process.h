#ifndef ECO_PERF_PROCESS_H_INCLUDED
#define ECO_PERF_PROCESS_H_INCLUDED

#include "cpu_usage.h"
#include "memory_usage.h"
#include <limits.h>

typedef struct ProcessData
{
    int valid;

    int pid;
    int directory;
    char state;
    char executable[NAME_MAX];

    int parent_pid;
    int pgroup;
    int psession;

    unsigned int flags;
    long priority;
    long nice_value;
    long num_threads;
    long long start_time;
    unsigned long long accumulated_io_delay;

    unsigned long start_stack;
    unsigned long stack_pointer;
    unsigned long instruction_pointer;

    cpu_core_data_t cpu_usage;
    memory_data_t memory_usage;
    unsigned long text_memory; // kB
    unsigned long data_memory; // kB
} process_data_t;

void init_process_data(process_data_t *process);

void read_process_data(process_data_t *process, int pid);

void print_process_data_summary(process_data_t const *process);

void get_process_command_line(
    char *destination,
    process_data_t const *process);

void list_processes();

void free_process_data(process_data_t *process);

#endif
