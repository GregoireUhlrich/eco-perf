#include "linear_layout.h"
#include "../twidget.h"
#include "compute_layout.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void _apply_hlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void _apply_vlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void init_twidget_linear_layout(
    twidget_linear_layout_t *layout,
    ct_direction_t direction)
{
    init_twidget_layout(layout);
    switch (direction)
    {
    case CT_HORIZONTAL:
        layout->apply_layout = _apply_hlayout;
        break;
    case CT_VERTICAL:
        layout->apply_layout = _apply_vlayout;
        break;
    default:
        errno = EINVAL;
        perror("Unknown layout linear direction.");
        exit(1);
    }
}

void _apply_hlayout(
    twidget_layout_t const *layout,
    twidget_t *widget)
{
    align_widget_for_linear_layout(
        widget,
        0, // Horizontal : x direction,
        &layout->config);
}

void _apply_vlayout(
    twidget_layout_t const *layout,
    twidget_t *widget)
{
    align_widget_for_linear_layout(
        widget,
        1, // Vertical : y direction,
        &layout->config);
}
