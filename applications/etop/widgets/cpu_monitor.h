#ifndef ECO_PERF_CPU_MONITOR_H_INCLUDED
#define ECO_PERF_CPU_MONITOR_H_INCLUDED

#include "eco_perf/metrics/cpu_usage.h"

#include "core_monitor.h"

typedef struct CPUMonitorTwidgetData
{
    cpu_data_t *cpu_data;
} cpu_monitor_twidget_data_t;

typedef struct CPUMonitorTwidgetConfig
{
    int first_core_monitor;
    int n_core_monitors;
} cpu_monitor_twidget_config_t;

typedef struct CPUMonitorTStack
{
    twidget_t twidget;
    twidget_linear_layout_t layout;

    core_monitor_tstack_t *core_monitors;

    cpu_monitor_twidget_data_t data;
    cpu_monitor_twidget_config_t config;
} cpu_monitor_tstack_t;

void init_cpu_monitor_twidget_data(cpu_monitor_twidget_data_t *data);

void init_cpu_monitor_twidget_config(cpu_monitor_twidget_config_t *config);

void init_cpu_monitor_tstack(cpu_monitor_tstack_t *monitor);

extern const twidget_interface_t cpu_monitor_twidget_interface;

#endif
