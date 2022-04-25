#include "hlayout.h"
#include "../twidget.h"
#include "compute_layout.h"

void _apply_hlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void init_twidget_hlayout(twidget_hlayout_t *layout)
{
    init_twidget_layout(layout);
    layout->apply_layout = _apply_hlayout;
}

void _apply_hlayout(
    twidget_layout_t const *layout,
    twidget_t *widget)
{
    align_widget_for_linear_layout(
        widget,
        0, // Horizontal : x direction,
        layout->config.auto_children_resize,
        layout->config.horizontal_align_mode);
}
