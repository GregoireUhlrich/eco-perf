#ifndef ECO_PERF_COMPUTE_LAYOUT_H_INCLUDED
#define ECO_PERF_COMPUTE_LAYOUT_H_INCLUDED

#include "../twidget.h"
#include "layout.h"

int compute_layout_stretching(
    twidget_t const *widget,
    int direction);

void align_widget_for_linear_layout(
    twidget_t *twidget,
    int direction,
    int resize,
    layout_align_mode_t alignement);

#endif
