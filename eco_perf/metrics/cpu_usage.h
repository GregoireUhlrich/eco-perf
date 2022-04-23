#ifndef ECO_PERF_CPU_USAGE_H_INCLUDED
#define ECO_PERF_CPU_USAGE_H_INCLUDED

typedef struct CPUCoreData
{
    union
    {
        double user_time;  // in s
        double user_ratio; // in [0, 1]
    };
    union
    {
        double sys_time;  // in s
        double sys_ratio; // in [0, 1]
    };
} cpu_core_data_t;

typedef struct CPUData
{
    int n_cpus;
    cpu_core_data_t *core_data;
} cpu_data_t;

void init_cpu_data(cpu_data_t *cpu_data);

void set_n_cpus(cpu_data_t *cpu_data, int n_cpus);

void read_cpu_data(cpu_data_t *cpu_data);

void print_cpu_data(cpu_data_t const *cpu_data);

void free_cpu_data(cpu_data_t *cpu_data);

void diff_cpu_data(
    cpu_data_t const *first,
    cpu_data_t const *last,
    cpu_data_t *diff);

void calculate_ratio(
    cpu_data_t const *diff,
    cpu_data_t *ratio,
    double n_sec);

#endif
