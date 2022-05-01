#include "twidget.h"
#include "../definitions/error.h"
#include "../terminal/cursor.h"
#include "layouts/compute_layout.h"
#include <stddef.h>
#include <string.h>

const twidget_interface_t default_twidget_interface = {
    default_twidget_update,
    default_twidget_draw,
    default_twidget_free};

twidget_update_function_t get_twidget_update_function(
    twidget_t const *widget)
{
    return widget->interface->update;
}

twidget_draw_function_t get_twidget_draw_function(
    twidget_t const *widget)
{
    return widget->interface->draw;
}

twidget_free_function_t get_twidget_free_function(
    twidget_t const *widget)
{
    return widget->interface->free;
}

void init_term_vector(terminal_vector_t *vector)
{
    vector->x = 0;
    vector->y = 0;
}

void init_tstack(tstack_t *stack)
{
    init_twidget(&stack->twidget);
}

void set_twidget_layout(
    twidget_t *widget,
    twidget_layout_t *layout)
{
    widget->layout = layout;
}

void apply_twidget_layout(twidget_t *widget)
{
    if (widget->floating)
    {
        place_floating_twidget(widget);
    }
    if (widget->layout && widget->layout->apply_layout)
    {
        widget->layout->apply_layout(widget->layout, widget);
    }
}

void init_twidget(twidget_t *widget)
{
    widget->hidden = 0;
    widget->floating = 0;

    init_term_vector(&widget->pos);
    init_term_vector(&widget->size);
    init_term_vector(&widget->fixed_size);

    widget->layout = NULL;

    widget->stack = NULL;
    widget->interface = &default_twidget_interface;

    widget->parent = NULL;
    es_vector_init(&widget->children);
}

void update_twidget(twidget_t *widget)
{
    const twidget_update_function_t update = get_twidget_update_function(widget);
    update(widget);
    apply_twidget_layout(widget);
    for (int i = 0; i != widget->children.size; ++i)
    {
        update_twidget((twidget_t *)widget->children.data[i]);
    }
}

int draw_twidget(twidget_t *widget)
{
#ifdef DISABLE_TERMINAL_DRAWING
    return 0;
#else
    const twidget_draw_function_t draw = get_twidget_draw_function(widget);
    if (widget && draw && !widget->hidden)
    {
        move_cursor_right(widget->pos.x);
        move_cursor_down(widget->pos.y);
        draw(widget);
        for (int i = 0; i != widget->children.size; ++i)
        {
            draw_twidget(widget->children.widgets[i]);
        }
        move_cursor_left(widget->pos.x);
        move_cursor_up(widget->pos.y);
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
    es_vector_push(&parent->children, child);
}

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child)
{
    for (int i = 0; i != parent->children.size; ++i)
        if (parent->children.data[i] == child)
            return i;
    return -1;
}

void remove_twidget_child(
    twidget_t *parent,
    twidget_t *child,
    int free_child)
{
    int pos = twidget_child_index(parent, child);
    CT_ASSERT(pos >= 0,
              CT_VALUE_ERROR,
              "Child not found in parent for removal.")
    if (free_child)
    {
        free_twidget(child);
    }
    child->parent = NULL;
    es_vector_erase(&parent->children, pos);
}

void free_twidget_children(twidget_t *widget)
{
    if (!widget)
    {
        return;
    }
    if (widget->children.data)
    {
        for (int i = 0; i != widget->children.size; ++i)
        {
            free_twidget((twidget_t *)widget->children.data[i]);
        }
        es_vector_free(&widget->children);
    }
}
void free_twidget(twidget_t *widget)
{
    if (!widget)
    {
        return;
    }
    free_twidget_children(widget);
    const twidget_free_function_t free_ = get_twidget_free_function(widget);
    if (free_)
    {
        free_(widget);
    }
}
