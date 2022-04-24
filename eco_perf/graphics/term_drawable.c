#include "term_drawable.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void init_term_vector(term_vector_t *vector)
{
    vector->x = 0;
    vector->y = 0;
}

void init_term_drawable(term_drawable_t *drawable)
{
    drawable->hidden = 0;
    init_term_vector(&drawable->top_left);
    init_term_vector(&drawable->size);
    drawable->n_children = 0;
    drawable->_memory_size = 0;
    drawable->children = NULL;
    drawable->draw_self = NULL;
}

int draw_term_drawable(term_drawable_t *drawable)
{
    if (drawable && drawable->draw_self && !drawable->hidden)
    {
        drawable->draw_self(drawable);
        for (int i = 0; i != drawable->n_children; ++i)
        {
            draw_term_drawable(drawable->children[i]);
        }
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

void free_term_drawable(term_drawable_t *drawable)
{
    for (int i = 0; i != drawable->n_children; ++i)
    {
        free_term_drawable(drawable->children[i]);
    }
    free(drawable->children);
}
