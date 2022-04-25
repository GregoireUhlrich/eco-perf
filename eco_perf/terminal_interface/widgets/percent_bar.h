#ifndef ECO_PERF_DRAWABLE_PERCNT_BAR_H_INCLUDED
#define ECO_PERF_DRAWABLE_PERCNT_BAR_H_INCLUDED

#include "../tools/percent_bar.h"
#include "twidget.h"

typedef twidget_t percent_bar_widget_t;

void init_percent_bar_twidget(
    percent_bar_widget_t *percent_bar,
    percent_bar_data_t *data,
    percent_bar_config_t *config);

#endif
