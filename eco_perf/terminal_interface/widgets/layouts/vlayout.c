#include "vlayout.h"
#include "compute_layout.h"

void _apply_vlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void init_twidget_vlayout(twidget_vlayout_t *layout)
{
    init_twidget_layout(layout);
    layout->apply_layout = _apply_vlayout;
}

void _apply_vlayout(
    twidget_layout_t const *layout,
    twidget_t *widget)
{
    align_widget_for_linear_layout(
        widget,
        1, // Vertical : y direction
        layout->config.auto_children_resize,
        layout->config.vertical_align_mode);
}
