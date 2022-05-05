#ifndef ETOP_PROC_MONITOR_H_INCLUDED
#define ETOP_PROC_MONITOR_H_INCLUDED

#include "eco_perf/cute_terminal/widgets/text_line.h"
#include "eco_perf/metrics/process_list.h"

typedef struct ProcMonitorTwidgetConfig
{

} proc_monitor_twidget_config_t;

typedef struct ProcMonitorTwidgetData
{
    process_list_t process_list;
} proc_monitor_twidget_data_t;

typedef struct ProcMonitorTStack
{
    twidget_t twidget;

    proc_monitor_twidget_data_t data;
    proc_monitor_twidget_config_t config;
} proc_monitor_tstack_t;

void init_proc_monitor_twidget_data(proc_monitor_twidget_data_t *monitor);
void init_proc_monitor_twidget_config(proc_monitor_twidget_config_t *monitor);

void init_proc_monitor_tstack(proc_monitor_tstack_t *stack);

extern const twidget_interface_t proc_monitor_twidget_interface;

#endif
