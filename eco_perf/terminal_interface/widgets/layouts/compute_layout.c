#include "compute_layout.h"

int compute_layout_stretching(
    twidget_t const *widget,
    int direction)
{
    int available_size = widget->size_v[direction];
    int n_stretchable_elements = 0;
    for (int i = 0; i != widget->children.size; ++i)
    {
        twidget_t const *child = widget->children.widgets[i];
        if (child->fixed_size_v[direction])
        {
            available_size -= child->size_v[direction];
        }
        else
        {
            ++n_stretchable_elements;
        }
    }
    if (n_stretchable_elements > 0)
        return available_size / n_stretchable_elements;
    return available_size;
}

void _apply_in_layout_direction(
    twidget_t *widget,
    int stretchable_size,
    int resize,
    int direction)
{
    int current_pos = 0;
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        twidget_t *child = children->widgets[i];
        if (child->floating)
        {
            continue;
        }
        child->pos_v[direction] = current_pos;
        if (resize && !child->fixed_size_v[direction])
        {
            child->size_v[direction] = stretchable_size;
        }
        current_pos += child->size_v[direction];
    }
}

void _apply_perpendicular_to_layout(
    twidget_t *widget,
    int direction,
    int resize,
    layout_align_mode_t alignement)
{
    const int perpendicular = 1 - direction;
    twidget_array_t *children = &widget->children;
    const int parent_size = widget->size_v[perpendicular];
    for (int i = 0; i != children->size; ++i)
    {
        twidget_t *child = children->widgets[i];
        if (child->floating)
        {
            return;
        }
        if (!resize || child->fixed_size_v[perpendicular])
        {
            const int size_delta = parent_size - child->size_v[perpendicular];
            if (alignement == LAYOUT_TWIDGET_CENTER)
            {
                child->pos_v[perpendicular] = size_delta / 2;
            }
            else if (alignement == LAYOUT_TWIDGET_BOTTOMRIGHT)
            {
                child->pos_v[perpendicular] = size_delta;
            }
            else
            {
                child->pos_v[perpendicular] = 0;
            }
        }
        else
        {
            child->size_v[perpendicular] = parent_size;
        }
    }
}

void align_widget_for_linear_layout(
    twidget_t *widget,
    int direction,
    int resize,
    layout_align_mode_t alignement)
{
    int stretchable_size = compute_layout_stretching(widget, direction);
    _apply_in_layout_direction(widget, stretchable_size, resize, direction);
    _apply_perpendicular_to_layout(widget, direction, resize, alignement);
}
