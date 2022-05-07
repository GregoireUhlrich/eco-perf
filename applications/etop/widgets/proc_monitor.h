#ifndef ETOP_PROC_MONITOR_H_INCLUDED
#define ETOP_PROC_MONITOR_H_INCLUDED

#include "eco_perf/cute_terminal/widgets/list.h"
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

    twidget_layout_t layout;
    proc_monitor_twidget_data_t data;
    proc_monitor_twidget_config_t config;

    list_tstack_t list;
    es_container_t lines;
} proc_monitor_tstack_t;

void proc_monitor_tstack_init(proc_monitor_tstack_t *stack);

extern const twidget_interface_t proc_monitor_twidget_interface;

#endif
