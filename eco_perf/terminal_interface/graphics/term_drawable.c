#include "term_drawable.h"
#include "../terminal/cursor.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_term_vector(term_vector_t *vector)
{
    vector->x = 0;
    vector->y = 0;
}

void _default_update(term_drawable_t *drawable)
{
}

int _default_draw(term_drawable_t const *drawable)
{
}

term_vector_t _default_get_origin(term_drawable_t const *drawable)
{
    static const term_vector_t default_origin = {0, 0};
    return default_origin;
}

void init_term_drawable(term_drawable_t *drawable)
{
    drawable->hidden = 0;
    init_term_vector(&drawable->pos);
    init_term_vector(&drawable->size);
    init_term_vector(&drawable->fixed_size);
    drawable->n_children = 0;
    drawable->_memory_size = 0;
    drawable->children = NULL;
    drawable->config = NULL;

    drawable->get_origin = _default_get_origin;
    drawable->update = _default_update;
    drawable->draw_self = _default_draw;
}

term_vector_t get_default_term_origin()
{
    term_vector_t origin = {0, 0};
    return origin;
}

void update_term_drawable(term_drawable_t *drawable)
{
    drawable->update(drawable);
    for (int i = 0; i != drawable->n_children; ++i)
    {
        update_term_drawable(drawable->children[i]);
    }
}

#include <unistd.h>
int draw_term_drawable(term_drawable_t *drawable)
{
    if (drawable && drawable->draw_self && !drawable->hidden)
    {
        term_vector_t origin = drawable->get_origin(drawable);
        const int offset_x = drawable->pos.x + origin.x;
        const int offset_y = drawable->pos.y + origin.y;
        move_cursor_right(offset_x);
        move_cursor_down(offset_y);
        drawable->draw_self(drawable);
        for (int i = 0; i != drawable->n_children; ++i)
        {
            draw_term_drawable(drawable->children[i]);
        }
        move_cursor_left(offset_x);
        move_cursor_up(offset_y);
        return 1;
    }
    return 0;
}

void _extend_memory_space(term_drawable_t *drawable)
{
    if (drawable->_memory_size == 0)
    {
        drawable->children = (term_drawable_t **)malloc(sizeof(term_drawable_t *));
        drawable->_memory_size = 1;
        return;
    }
    term_drawable_t **new_array = malloc(
        2 * drawable->_memory_size * sizeof(term_drawable_t *));
    memcpy(
        new_array,
        drawable->children,
        drawable->n_children * sizeof(term_drawable_t *));
    free(drawable->children);
    drawable->_memory_size *= 2;
    drawable->children = new_array;
}

void add_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child)
{
    if (parent->n_children == parent->_memory_size)
    {
        _extend_memory_space(parent);
    }
    parent->children[parent->n_children] = child;
    ++parent->n_children;
}

int index_of_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child)
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

void remove_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child,
    int release_child)
{
    int index = index_of_term_drawable_child(parent, child);
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
        free_term_drawable(child);
    }
}

void free_term_drawable(term_drawable_t *drawable)
{
    for (int i = 0; i != drawable->n_children; ++i)
    {
        free_term_drawable(drawable->children[i]);
    }
    free(drawable->children);
}
