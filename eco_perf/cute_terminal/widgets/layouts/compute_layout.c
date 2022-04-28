#include "compute_layout.h"
#include "../../definitions/error.h"

void _compute_layout_stretching(
    twidget_t const *widget,
    int direction,
    twidget_layout_config_t const *config,
    int *stretchable_size,
    int *n_stretchable_elements)
{
    if (!config->auto_children_resize)
    {
        *stretchable_size = 0;
        *n_stretchable_elements = 0;
        return;
    }
    int available_size = widget->size_v[direction];
    available_size -= config->spacing * (widget->children.size - 1);
    *n_stretchable_elements = 0;
    for (int i = 0; i != widget->children.size; ++i)
    {
        twidget_t const *child = widget->children.widgets[i];
        if (child->fixed_size_v[direction])
        {
            available_size -= child->size_v[direction];
        }
        else if (!child->floating)
        {
            ++*n_stretchable_elements;
        }
    }
    *stretchable_size = available_size;
}

void _apply_in_layout_direction(
    twidget_t *widget,
    int stretchable_size,
    int direction,
    twidget_layout_config_t const *config)
{
    int alignement = (direction == 0) ? config->horizontal_align_mode
                                      : config->vertical_align_mode;
    int current_pos = 0;
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        twidget_t *child = children->widgets[i];
        if (child->floating)
        {
            continue;
        }
        if (!child->fixed_size_v[direction] && config->auto_children_resize)
        {
            unsigned int *child_size = &child->size_v[direction];
            if (!*child_size || *child_size > stretchable_size)
            {
                // Size invalid or too big: adapt to layout
                *child_size = stretchable_size;
                child->pos_v[direction] = current_pos;
                current_pos += stretchable_size;
            }
            else
            {
                // Good size, we apply simply the alignement
                int delta_size = stretchable_size - *child_size;
                switch (alignement)
                {
                case CT_CENTER:
                    child->pos_v[direction] = current_pos + delta_size / 2;
                    current_pos += stretchable_size;
                    break;
                case CT_BOTTOM_OR_RIGHT:
                    child->pos_v[direction] = current_pos + delta_size;
                    current_pos += stretchable_size;
                    break;
                default:
                    child->pos_v[direction] = current_pos;
                    current_pos += stretchable_size;
                    break;
                }
            }
        }
        else
        {
            child->pos_v[direction] = current_pos;
            current_pos += child->size_v[direction];
        }
        current_pos += config->spacing;
    }
}

void _align_fixed_elements_on_top(
    twidget_t *widget,
    int direction,
    twidget_layout_config_t const *config)
{
    int current_pos = 0;
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        if (children->widgets[i]->floating)
            continue;
        twidget_t *child = children->widgets[i];
        child->pos_v[direction] = current_pos;
        current_pos += child->size_v[direction] + config->spacing;
    }
}

void _center_fixed_elements(
    twidget_t *widget,
    int direction,
    int n_fixed_size_children,
    twidget_layout_config_t const *config)
{
    int current_pos = 0;
    int total_stretchable_size = widget->size_v[direction];
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        if (children->widgets[i]->floating)
            continue;
        twidget_t *child = children->widgets[i];
        int size_expand_into = total_stretchable_size / n_fixed_size_children - config->spacing;
        int delta_size = size_expand_into - child->size_v[direction];
        if (delta_size > 0)
        {
            child->pos_v[direction] = current_pos + delta_size / 2;
            current_pos += size_expand_into + config->spacing;
            total_stretchable_size -= size_expand_into + config->spacing;
        }
        else
        {
            child->pos_v[direction] = current_pos;
            current_pos += child->size_v[direction] + config->spacing;
            total_stretchable_size -= child->size_v[direction] + config->spacing;
        }
        --n_fixed_size_children;
    }
}

void _align_fixed_elements_at_bottom(
    twidget_t *widget,
    int direction,
    int total_fixed_size,
    int n_fixed_size_children,
    twidget_layout_config_t const *config)
{
    int total_stretchable_size = widget->size_v[direction];
    int current_pos = total_stretchable_size - total_fixed_size - (n_fixed_size_children - 1) * config->spacing;
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        if (children->widgets[i]->floating)
            continue;
        twidget_t *child = children->widgets[i];
        child->pos_v[direction] = current_pos;
        current_pos += child->size_v[direction] + config->spacing;
    }
}

void _apply_fixed_in_layout_direction(
    twidget_t *widget,
    int direction,
    twidget_layout_config_t const *config)
{
    // Get necessary variables
    int alignement = (direction == 0) ? config->horizontal_align_mode
                                      : config->vertical_align_mode;
    int total_fixed_size = 0;
    int n_fixed_size_children = 0;
    twidget_array_t *children = &widget->children;
    for (int i = 0; i != children->size; ++i)
    {
        if (!children->widgets[i]->floating)
        {
            ++n_fixed_size_children;
            total_fixed_size += children->widgets[i]->size_v[direction];
        }
    }

    // Align elements following policy
    switch (alignement)
    {
    case CT_TOP_OR_LEFT:
        _align_fixed_elements_on_top(widget, direction, config);
        break;
    case CT_BOTTOM_OR_RIGHT:
        _align_fixed_elements_at_bottom(
            widget,
            direction,
            total_fixed_size,
            n_fixed_size_children,
            config);
        break;
    case CT_CENTER:
        _center_fixed_elements(
            widget,
            direction,
            n_fixed_size_children,
            config);
        break;
    default:
        break;
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
            continue;
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
    int stretchable_size, n_stretchable_elements;
    _compute_layout_stretching(
        widget, direction, config,
        &stretchable_size, &n_stretchable_elements);
    if (n_stretchable_elements == 0)
    {
        _apply_fixed_in_layout_direction(
            widget,
            direction,
            config);
    }
    else
    {
        _apply_in_layout_direction(
            widget,
            stretchable_size / n_stretchable_elements,
            direction,
            config);
    }
    _apply_perpendicular_to_layout(
        widget, direction, config);
}

void place_floating_twidget(
    twidget_t *twidget)
{
    CT_ASSERT(
        twidget->size.x > 0 && twidget->size.y > 0,
        CT_VALUE_ERROR,
        "The size of floating widgets must be defined. "
        "Found size (%d, %d) for twidget at address %p.",
        twidget->size.x,
        twidget->size.y,
        twidget)
}
