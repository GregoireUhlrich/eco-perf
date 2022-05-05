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
    union
    {
        double nice_time;  // in s
        double nice_ratio; // in [0, 1]
    };

} cpu_core_data_t;

typedef struct CPUData
{
    int n_cpus;
    cpu_core_data_t *core_data;
} cpu_data_t;

void cpu_data_init(cpu_data_t *cpu_data);

void cpu_data_set_ncpus(cpu_data_t *cpu_data, int n_cpus);

void cpu_data_read(cpu_data_t *cpu_data);

void cpu_data_print(cpu_data_t const *cpu_data);

void cpu_data_free(cpu_data_t *cpu_data);

void cpu_data_diff(
    cpu_data_t const *first,
    cpu_data_t const *last,
    cpu_data_t *diff);

void cpu_data_ratio(
    cpu_data_t const *diff,
    cpu_data_t *ratio,
    double n_sec);

#endif
