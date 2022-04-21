#ifndef ECO_PERF_CPU_USAGE_H_INCLUDED
#define ECO_PERF_CPU_USAGE_H_INCLUDED

struct cpu_core_data
{
    double user_time; // in s
    double sys_time;  // in s
};

typedef struct cpu_core_data cpu_core_data_t;

struct cpu_data
{
    int n_cpus;
    cpu_core_data_t *core_data;
};

typedef struct cpu_data cpu_data_t;

void read_cpu_data(cpu_data_t *cpu_data);

void print_cpu_data(cpu_data_t const *cpu_data);

void free_cpu_data(cpu_data_t *cpu_data);

void diff_cpu_data(
    cpu_data_t const *first,
    cpu_data_t const *last,
    cpu_data_t *diff);

#endif
