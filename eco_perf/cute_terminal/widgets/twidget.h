#ifndef CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED
#define CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED

#include "../terminal/vector.h"
#include "../tools/twidget_array.h"
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
    twidget_array_t children;
} twidget_t;

void init_twidget(twidget_t *widget);

void set_twidget_layout(
    twidget_t *widget,
    twidget_layout_t *layout);

void update_twidget(twidget_t *widget);

int draw_twidget(twidget_t *widget);

void free_twidget_children(twidget_t *widget);

void free_twidget(twidget_t *widget);

void apply_twidget_layout(twidget_t *widget);

void add_twidget_child(
    twidget_t *parent,
    twidget_t *child);

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child);

void remove_twidget_child(
    twidget_t *parent,
    twidget_t *child,
    int free_child);

extern const twidget_interface_t default_twidget_interface;

typedef struct TStack
{
    twidget_t twidget;
} tstack_t;

void init_tstack(tstack_t *stack);

#undef DEF_TERMINAL_VECTOR

#endif
