#ifndef ECO_PERF_DRAWABLE_PERCNT_BAR_H_INCLUDED
#define ECO_PERF_DRAWABLE_PERCNT_BAR_H_INCLUDED

#include "percent_bar.h"
#include "twidget.h"

typedef twidget_t drawable_percent_bar_t;

void init_drawable_percent_bar(
    drawable_percent_bar_t *percent_bar,
    percent_bar_data_t *data,
    percent_bar_config_t *config);

#endif
