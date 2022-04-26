#ifndef CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED
#define CUTE_TERMINAL_TERM_DRAWABLE_H_INCLUDED

#include "../terminal/vector.h"
#include "../tools/twidget_array.h"
#include "layouts/layout.h"

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

    twidget_layout_t *layout;

    DEF_TERMINAL_VECTOR(pos);
    DEF_TERMINAL_VECTOR(size);
    DEF_TERMINAL_VECTOR(fixed_size);

    struct TWidget *parent;
    twidget_array_t children;

    void *config; // possible additional config
    void *data;   // possible additional data

    void (*update)(struct TWidget *widget);

    terminal_vector_t (*get_origin)(
        struct TWidget const *widget);

    int (*draw_self)(struct TWidget const *widget);
} twidget_t;

void init_twidget(twidget_t *widget);

terminal_vector_t get_default_twidget_origin();

void set_twidget_layout(
    twidget_t *widget,
    twidget_layout_t *layout);

void apply_twidget_layout(twidget_t *widget);

void update_twidget(twidget_t *widget);

int draw_twidget(twidget_t *widget);

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

void free_twidget(twidget_t *widget);

#undef DEF_TERMINAL_VECTOR

#endif
