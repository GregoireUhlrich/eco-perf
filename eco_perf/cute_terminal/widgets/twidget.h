#ifndef CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED
#define CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED

#include "../../eco_std/vector.h"
#include "../terminal/vector.h"
#include "layouts/layout.h"
#include "twidget_interface.h"

#define DEF_TERMINAL_VECTOR(name) \
    union                         \
    {                             \
        terminal_vector_t name;   \
        unsigned int name##_v[2]; \
    }

typedef struct TWidget
{
    int hidden;
    int floating;

    DEF_TERMINAL_VECTOR(pos);
    DEF_TERMINAL_VECTOR(size);
    DEF_TERMINAL_VECTOR(fixed_size);

    twidget_layout_t *layout; // layout for the widget, can be null

    void *stack;
    twidget_interface_t const *interface; // interface functions

    struct TWidget *parent;
    es_vector_t children;
} twidget_t;

void twidget_init(twidget_t *widget);

void twidget_set_layout(
    twidget_t *widget,
    twidget_layout_t *layout);

void update_twidget(twidget_t *widget);

int twidget_draw(twidget_t *widget);

void twidget_free_children(twidget_t *widget);

void twidget_free(twidget_t *widget);

void twidget_apply_layout(twidget_t *widget);

void twidget_add_child(
    twidget_t *parent,
    twidget_t *child);

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child);

void twidget_remove_child(
    twidget_t *parent,
    twidget_t *child,
    int free_child);

extern const twidget_interface_t default_twidget_interface;

typedef struct TStack
{
    twidget_t twidget;
} tstack_t;

void tstack_init(tstack_t *stack);

#undef DEF_TERMINAL_VECTOR

#endif
