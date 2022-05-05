#ifndef CUTE_TERMINAL_TWIDGET_HLAYOUT_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_HLAYOUT_H_INCLUDED

#include "../../definitions/direction.h"
#include "layout.h"

typedef twidget_layout_t twidget_linear_layout_t;

void twidget_linear_layout_init(
    twidget_linear_layout_t *layout,
    ct_direction_t direction);

#endif
