#include "cpu_usage.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

void init_cpu_data(cpu_data_t *cpu_data)
{
    cpu_data->n_cpus = 0;
    cpu_data->core_data = NULL;
}

void set_n_cpus(cpu_data_t *cpu_data, int n_cpus)
{
    if (cpu_data->core_data != NULL)
    {
        if (cpu_data->n_cpus == n_cpus)
        {
            return;
        }
        free(cpu_data->core_data);
    }
    if (n_cpus > 0)
    {
        cpu_data->n_cpus = n_cpus;
        cpu_data->core_data = malloc(n_cpus * sizeof(cpu_core_data_t));
    }
}

int _count_cpu_data(FILE *file)
{
    int n_cpu = 0;
    char buff[255];
    int cpu_section = 0;
    int current_pos = ftell(file);
    int cpu_section_pos = 0;
    while (fscanf(file, "%s", buff) != EOF)
    {
        if (cpu_section && strncmp(buff, "cpu", 3) == 0)
        {
            ++n_cpu;
        }
        else if (strcmp(buff, "cpu") == 0)
        {
            cpu_section_pos = current_pos;
            cpu_section = 1;
        }
        current_pos = ftell(file);
    }
    fseek(file, cpu_section_pos, 0);
    return n_cpu;
}

void _read_cpu_core_data_line(FILE *file, cpu_core_data_t *core_data)
{
    char cpu_def[10];
    long int user_time, nice_time, system_time, idle_time, io_wait,
        ireq, softirq, steal, guest, guest_nice;
    fscanf(file, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
           cpu_def, &user_time, &nice_time, &system_time, &idle_time,
           &io_wait, &ireq, &softirq, &steal, &guest, &guest_nice);
    core_data->sys_time = system_time / 100.;
    core_data->user_time = user_time / 100.;
}

void _read_cpu_data(FILE *file, cpu_data_t *cpu_data)
{
    int n_cpu = _count_cpu_data(file);
    set_n_cpus(cpu_data, n_cpu);
    if (n_cpu > 0)
    {
        cpu_core_data_t core_data;
        _read_cpu_core_data_line(file, &core_data);
        for (int i = 0; i != n_cpu; ++i)
        {
            _read_cpu_core_data_line(file, &cpu_data->core_data[i]);
        }
    }
}

void read_cpu_data(cpu_data_t *cpu_data)
{
    FILE *proc_stats = fopen("/proc/stat", "r");
    if (!proc_stats)
    {
        perror("Cannot open file /proc/stat");
        exit(1);
    }
    _read_cpu_data(proc_stats, cpu_data);
    fclose(proc_stats);
}

void print_cpu_data(cpu_data_t const *cpu_data)
{
    printf("CPU data for %d CPUs\n", cpu_data->n_cpus);
    for (int i = 0; i != cpu_data->n_cpus; ++i)
    {
        printf(
            "  -> cpu %d: sys=%.2f, usr=%.2f\n",
            i,
            cpu_data->core_data[i].sys_time,
            cpu_data->core_data[i].user_time);
    }
}

void free_cpu_data(cpu_data_t *cpu_data)
{
    free(cpu_data->core_data);
    init_cpu_data(cpu_data);
}

void diff_cpu_data(
    cpu_data_t const *first,
    cpu_data_t const *last,
    cpu_data_t *diff)
{
    if (first->n_cpus != last->n_cpus)
    {
        errno = EINVAL;
        char error[100];
        sprintf(
            error,
            "Cannot diff cpu data with different number of cpus: %d and %d.",
            first->n_cpus,
            last->n_cpus);
        perror(error);
        exit(1);
    }
    if (first->n_cpus <= 0)
    {
        errno = EINVAL;
        perror("Diff cannot be calculated for a non-positive cpu number.");
        exit(1);
    }
    set_n_cpus(diff, first->n_cpus);
    for (int i = 0; i != diff->n_cpus; ++i)
    {
        diff->core_data[i].sys_time =
            last->core_data[i].sys_time - first->core_data[i].sys_time;
        diff->core_data[i].user_time =
            last->core_data[i].user_time - first->core_data[i].user_time;
    }
}

void calculate_ratio(cpu_data_t const *cpu_data, cpu_data_t *ratio, double n_sec)
{
    if (cpu_data->n_cpus > 0)
    {
        set_n_cpus(ratio, cpu_data->n_cpus);
        for (int i = 0; i != ratio->n_cpus; ++i)
        {
            ratio->core_data[i].user_ratio = cpu_data->core_data[i].user_time / n_sec;
            ratio->core_data[i].sys_ratio = cpu_data->core_data[i].sys_time / n_sec;
        }
    }
    else
    {
        errno = EINVAL;
        perror("Cannot calculate ratio for a non-positive cpu number.");
        exit(1);
    }
}
