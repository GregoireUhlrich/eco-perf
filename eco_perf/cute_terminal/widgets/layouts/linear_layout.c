#include "linear_layout.h"
#include "../../definitions/error.h"
#include "../twidget.h"
#include "compute_layout.h"

void _apply_hlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void _apply_vlayout(
    twidget_layout_t const *layout,
    twidget_t *widget);

void twidget_linear_layout_init(
    twidget_linear_layout_t *layout,
    ct_direction_t direction)
{
    twidget_layout_init(layout);
    switch (direction)
    {
    case CT_HORIZONTAL:
        layout->apply_layout = _apply_hlayout;
        break;
    case CT_VERTICAL:
        layout->apply_layout = _apply_vlayout;
        break;
    default:
        CT_ERROR(
            CT_VALUE_ERROR,
            "Unknown linear direction for layout %d.",
            direction)
    }
}

twidget_linear_layout_t twidget_layout_config_create(
    ct_direction_t direction)
{
    twidget_linear_layout_t layout;
    twidget_linear_layout_init(&layout, direction);
    return layout;
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
