#include "compute_layout.h"

int compute_layout_stretching(
    twidget_t const *widget,
    int direction,
    twidget_layout_config_t const *config)
{
    int available_size = widget->size_v[direction];
    available_size -= config->spacing * (widget->children.size - 1);
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
    int direction,
    twidget_layout_config_t const *config)
{
    int current_pos = 0;
    twidget_array_t *children = &widget->children;
    const int alignement =
        (direction == 0) ? config->horizontal_align_mode
                         : config->vertical_align_mode;
    for (int i = 0; i != children->size; ++i)
    {
        twidget_t *child = children->widgets[i];
        if (child->floating)
        {
            continue;
        }
        child->pos_v[direction] = current_pos;
        if (!child->fixed_size_v[direction])
        {
            int resize = child->size_v[direction] > stretchable_size || !child->size_v[direction];
            resize = resize && config->auto_children_resize;
            if (resize)
            {
                // Size invalid or too big: adapt to layout
                child->size_v[direction] = stretchable_size;
            }
            else
            {
                // If element smaller than block size, realign
                int diff_size = stretchable_size - child->size_v[direction];
                switch (alignement)
                {
                case CT_CENTER:
                    child->pos_v[direction] += diff_size / 2;
                    break;
                case CT_BOTTOM_OR_RIGHT:
                    child->pos_v[direction] += diff_size;
                    break;
                default:
                    break;
                }
            }
            current_pos += stretchable_size;
        }
        else
        {
            current_pos += child->size_v[direction];
        }
        current_pos += config->spacing;
    }
}

void _apply_perpendicular_to_layout(
    twidget_t *widget,
    int direction,
    twidget_layout_config_t const *config)
{
    const int perpendicular = 1 - direction;
    twidget_array_t *children = &widget->children;
    const int parent_size = widget->size_v[perpendicular];
    const int alignement =
        (direction == 1) ? config->horizontal_align_mode
                         : config->vertical_align_mode;
    for (int i = 0; i != children->size; ++i)
    {
        twidget_t *child = children->widgets[i];
        if (child->floating)
        {
            return;
        }
        if (!config->auto_children_resize || child->fixed_size_v[perpendicular])
        {
            const int size_delta = parent_size - child->size_v[perpendicular];
            if (alignement == CT_CENTER)
            {
                child->pos_v[perpendicular] = size_delta / 2;
            }
            else if (alignement == CT_BOTTOM_OR_RIGHT)
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
    twidget_layout_config_t const *config)
{
    int stretchable_size = compute_layout_stretching(
        widget, direction, config);
    _apply_in_layout_direction(
        widget, stretchable_size, direction, config);
    _apply_perpendicular_to_layout(
        widget, direction, config);
}
