#include "layout.h"
#include "../twidget.h"
#include <stdlib.h>

void init_twidget_layout_config(twidget_layout_config_t *config)
{
    config->auto_children_resize = 1;
    config->horizontal_align_mode = CT_CENTER;
    config->vertical_align_mode = CT_CENTER;
    config->spacing = 0;
}

void init_twidget_layout(twidget_layout_t *layout)
{
    init_twidget_layout_config(&layout->config);
    layout->apply_layout = NULL;
}

void _apply_default_twidget_layout(
    twidget_layout_t const *layout,
    twidget_t *widget)
{
    if (widget->size.x == 0 || widget->size.y == 0 && widget->parent)
    {
        // If null size inherit from parent (when possible)
        widget->size = widget->parent->size;
    }
}

twidget_layout_t default_twidget_layout()
{
    twidget_layout_t layout;
    init_twidget_layout(&layout);
    layout.apply_layout = _apply_default_twidget_layout;
    return layout;
}
