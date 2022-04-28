#ifndef CUTE_TERMINAL_COMPUTE_LAYOUT_H_INCLUDED
#define CUTE_TERMINAL_COMPUTE_LAYOUT_H_INCLUDED

#include "../twidget.h"
#include "layout.h"

void align_widget_for_linear_layout(
    twidget_t *twidget,
    int direction,
    twidget_layout_config_t const *config);

void place_floating_twidget(
    twidget_t *twidget);

#endif
