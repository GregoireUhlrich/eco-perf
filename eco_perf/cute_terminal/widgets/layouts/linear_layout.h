#ifndef ECO_PERF_TWIDGET_HLAYOUT_H_INCLUDED
#define ECO_PERF_TWIDGET_HLAYOUT_H_INCLUDED

#include "../../definitions/direction.h"
#include "layout.h"

typedef twidget_layout_t twidget_linear_layout_t;

void init_twidget_linear_layout(
    twidget_linear_layout_t *layout,
    ct_direction_t direction);

#endif
