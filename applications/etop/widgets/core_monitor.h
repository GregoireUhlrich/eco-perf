#ifndef ETOP_CPU_INFO_TWIDGET_H_INCLUDED
#define ETOP_CPU_INFO_TWIDGET_H_INCLUDED

#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include "eco_perf/metrics/cpu_usage.h"

#define MAX_CORE_MONITOR_TITLE_SIZE 15
typedef struct CoreMonitorTwidgetData
{
    char title[MAX_CORE_MONITOR_TITLE_SIZE + 1];
    cpu_core_data_t *core_data;
} core_monitor_twidget_data_t;

typedef struct CoreMonitorTwidgetConfig
{
} core_monitor_twidget_config_t;

typedef struct CoreMonitorTStack
{
    twidget_t twidget;

    twidget_linear_layout_t layout;

    text_line_tstack_t title;
    percent_bar_tstack_t percent_bar;

    core_monitor_twidget_data_t data;
    core_monitor_twidget_config_t config;
} core_monitor_tstack_t;

void init_core_monitor_twidget_data(core_monitor_twidget_data_t *data);

void init_core_monitor_twidget_config(core_monitor_twidget_config_t *config);

void init_core_monitor_tstack(core_monitor_tstack_t *stack);

void free_core_monitor_stack(core_monitor_tstack_t *monitor);

void set_core_monitor_data(
    core_monitor_tstack_t *stack,
    cpu_core_data_t *core_data);

void set_core_monitor_title(
    core_monitor_tstack_t *stack,
    char const *title);

extern const twidget_interface_t core_monitor_twidget_interface;

#endif
