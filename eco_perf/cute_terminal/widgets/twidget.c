#include "twidget.h"
#include "../definitions/error.h"
#include "../terminal/cursor.h"
#include <stddef.h>
#include <string.h>

void init_term_vector(terminal_vector_t *vector)
{
    vector->x = 0;
    vector->y = 0;
}

void set_twidget_layout(
    twidget_t *widget,
    twidget_layout_t *layout)
{
    widget->layout = layout;
}

void apply_twidget_layout(twidget_t *widget)
{
    if (widget->layout && widget->layout->apply_layout)
    {
        widget->layout->apply_layout(widget->layout, widget);
    }
}

void _default_update(twidget_t *widget)
{
    apply_twidget_layout(widget);
}

int _default_draw(twidget_t const *widget)
{
    return 0;
}

terminal_vector_t _default_get_origin(twidget_t const *widget)
{
    static const terminal_vector_t default_origin = {0, 0};
    return default_origin;
}

void init_twidget(twidget_t *widget)
{
    widget->hidden = 0;
    widget->floating = 0;
    widget->parent = NULL;
    widget->layout = NULL;
    init_term_vector(&widget->pos);
    init_term_vector(&widget->size);
    init_term_vector(&widget->fixed_size);
    init_twidget_array(&widget->children);
    widget->config = NULL;

    widget->get_origin = _default_get_origin;
    widget->update = _default_update;
    widget->draw_self = _default_draw;
}

terminal_vector_t get_default_twidget_origin()
{
    terminal_vector_t origin = {0, 0};
    return origin;
}

void update_twidget(twidget_t *widget)
{
    widget->update(widget);
    for (int i = 0; i != widget->children.size; ++i)
    {
        update_twidget(widget->children.widgets[i]);
    }
}

int draw_twidget(twidget_t *widget)
{
#ifdef DISABLE_TERMINAL_DRAWING
    return 0;
#else
    if (widget && widget->draw_self && !widget->hidden)
    {
        terminal_vector_t origin = widget->get_origin(widget);
        const int offset_x = widget->pos.x + origin.x;
        const int offset_y = widget->pos.y + origin.y;
        move_cursor_right(offset_x);
        move_cursor_down(offset_y);
        widget->draw_self(widget);
        for (int i = 0; i != widget->children.size; ++i)
        {
            draw_twidget(widget->children.widgets[i]);
        }
        move_cursor_left(offset_x);
        move_cursor_up(offset_y);
        return 1;
    }
    return 0;
#endif
}

void add_twidget_child(
    twidget_t *parent,
    twidget_t *child)
{
    CT_ASSERT(
        !child->parent,
        CT_VALUE_ERROR,
        "Child already has a parent!")
    child->parent = parent;
    twidget_array_push_back(&parent->children, child);
}

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child)
{
    return twidget_array_index_of(&parent->children, child);
}

void remove_twidget_child(
    twidget_t *parent,
    twidget_t *child,
    int free_child)
{
    if (free_child)
    {
        free_twidget(child);
    }
    else
    {
        child->parent = NULL;
    }
    twidget_array_remove(&parent->children, child);
}

void free_twidget(twidget_t *widget)
{
    if (!widget->children.widgets)
    {
        return;
    }
    for (int i = 0; i != widget->children.size; ++i)
    {
        free_twidget(widget->children.widgets[i]);
    }
    free_twidget_array(&widget->children);
}
