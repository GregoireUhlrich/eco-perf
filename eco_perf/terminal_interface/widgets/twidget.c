#include "twidget.h"
#include "../terminal/cursor.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_term_vector(terminal_vector_t *vector)
{
    vector->x = 0;
    vector->y = 0;
}

void _default_update(twidget_t *widget)
{
}

int _default_draw(twidget_t const *widget)
{
}

terminal_vector_t _default_get_origin(twidget_t const *widget)
{
    static const terminal_vector_t default_origin = {0, 0};
    return default_origin;
}

void init_twidget(twidget_t *widget)
{
    widget->hidden = 0;
    init_term_vector(&widget->pos);
    init_term_vector(&widget->size);
    init_term_vector(&widget->fixed_size);
    widget->n_children = 0;
    widget->_memory_size = 0;
    widget->children = NULL;
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
    for (int i = 0; i != widget->n_children; ++i)
    {
        update_twidget(widget->children[i]);
    }
}

int draw_twidget(twidget_t *widget)
{
    if (widget && widget->draw_self && !widget->hidden)
    {
        terminal_vector_t origin = widget->get_origin(widget);
        const int offset_x = widget->pos.x + origin.x;
        const int offset_y = widget->pos.y + origin.y;
        move_cursor_right(offset_x);
        move_cursor_down(offset_y);
        widget->draw_self(widget);
        for (int i = 0; i != widget->n_children; ++i)
        {
            draw_twidget(widget->children[i]);
        }
        move_cursor_left(offset_x);
        move_cursor_up(offset_y);
        return 1;
    }
    return 0;
}

void _extend_memory_space(twidget_t *widget)
{
    if (widget->_memory_size == 0)
    {
        widget->children = (twidget_t **)malloc(sizeof(twidget_t *));
        widget->_memory_size = 1;
        return;
    }
    twidget_t **new_array = malloc(
        2 * widget->_memory_size * sizeof(twidget_t *));
    memcpy(
        new_array,
        widget->children,
        widget->n_children * sizeof(twidget_t *));
    free(widget->children);
    widget->_memory_size *= 2;
    widget->children = new_array;
}

void add_twidget_child(
    twidget_t *parent,
    twidget_t *child)
{
    if (parent->n_children == parent->_memory_size)
    {
        _extend_memory_space(parent);
    }
    parent->children[parent->n_children] = child;
    ++parent->n_children;
}

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child)
{
    for (int i = 0; i != parent->n_children; ++i)
    {
        if (parent->children[i] == child)
        {
            return i;
        }
    }
    return -1;
}

void remove_twidget_child(
    twidget_t *parent,
    twidget_t *child,
    int release_child)
{
    int index = twidget_child_index(parent, child);
    if (index == -1)
    {
        errno = EINVAL;
        perror("Child not found in parent for removal.");
        exit(1);
    }
    for (int i = index; i != parent->n_children - 1; ++i)
    {
        parent->children[i] = parent->children[i + 1];
    }
    --parent->n_children;
    if (release_child)
    {
        free_twidget(child);
    }
}

void free_twidget(twidget_t *widget)
{
    if (!widget->children)
    {
        return;
    }
    for (int i = 0; i != widget->n_children; ++i)
    {
        free_twidget(widget->children[i]);
    }
    free(widget->children);
    widget->children = NULL;
    widget->n_children = 0;
}
