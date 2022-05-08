#ifndef ECO_PERF_PROCESS_H_INCLUDED
#define ECO_PERF_PROCESS_H_INCLUDED

#include "cpu_usage.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/es_string.h"
#include "memory_usage.h"
#include <limits.h>
#include <stddef.h>
#include <time.h>

typedef struct ProcessData
{
    int valid;
    int directory;

    int pid;
    char state;
    es_string_t executable;

    int parent_pid;
    int pgroup;
    int psession;

    unsigned int flags;
    long priority;
    long nice_value;
    long num_threads;
    long long start_time;

    unsigned long start_stack;
    unsigned long stack_pointer;
    unsigned long instruction_pointer;

    struct timespec prev_time; // last recording time
    struct timespec time;      // previous recorded time
    cpu_core_data_t prev_cpu_usage;
    cpu_core_data_t cpu_usage;
    memory_data_t memory_usage;
} process_data_t;

void process_data_init(process_data_t *process);

void process_data_read(process_data_t *process, int pid);

void process_data_summary(process_data_t *process);

void process_data_get_cmdline(
    char *destination,
    process_data_t *process);

es_size_t process_data_get_time_delta_ms(
    process_data_t *process);

memory_data_t process_data_get_cumulative_memory_s(
    process_data_t *process);

cpu_core_data_t process_data_get_cpu_diff(
    process_data_t *process);

void list_processes();

void process_data_free(process_data_t *process);

#endif
