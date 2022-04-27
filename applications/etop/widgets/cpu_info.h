#ifndef ETOP_CPU_INFO_TWIDGET_H_INCLUDED
#define ETOP_CPU_INFO_TWIDGET_H_INCLUDED

#include "eco_perf/cute_terminal/widgets/layouts/linear_layout.h"
#include "eco_perf/cute_terminal/widgets/percent_bar.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include "eco_perf/metrics/cpu_usage.h"

typedef twidget_t cpu_info_twidget_t;

typedef struct CPUInfoTwidgetData
{
    char const *title;
    cpu_core_data_t *cpu_data;
    percent_bar_data_t percent_bar_data;
} cpu_info_twidget_data_t;

typedef struct CPUInfoTwidgetConfig
{
    twidget_linear_layout_t layout;
    percent_bar_config_t percent_bar_config;
} cpu_info_twidget_config_t;

typedef struct CPUInfoTwidgetContainer
{
    // Main widget
    twidget_t widget;

    // Sub-widgets
    text_line_twidget_t title_widget;
    cpu_info_twidget_t cpu_widget;

    // Data and config
    cpu_info_twidget_data_t data;
    cpu_info_twidget_config_t config;

} cpu_info_twidget_container_t;

void init_cpu_info_twidget_container(
    cpu_info_twidget_container_t *container);

void set_cpu_info_twidget_data(
    cpu_info_twidget_t *widget,
    cpu_core_data_t *data);

#endif
