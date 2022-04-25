#ifndef ECO_PERF_COMPUTE_LAYOUT_H_INCLUDED
#define ECO_PERF_COMPUTE_LAYOUT_H_INCLUDED

#include "../twidget.h"
#include "layout.h"

int compute_layout_stretching(
    twidget_t const *widget,
    int direction,
    twidget_layout_config_t const *config);

void align_widget_for_linear_layout(
    twidget_t *twidget,
    int direction,
    twidget_layout_config_t const *config);

#endif
