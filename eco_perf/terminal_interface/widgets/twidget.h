#ifndef ECO_PERF_TERM_DRAWABLE_H_INCLUDED
#define ECO_PERF_TERM_DRAWABLE_H_INCLUDED

#include "../terminal/vector.h"
typedef struct TWidget
{
    int hidden;
    union
    {
        terminal_vector_t pos;
        unsigned int pos_v[2];
    };
    union
    {
        terminal_vector_t size;
        unsigned int size_v[2];
    };
    union
    {
        terminal_vector_t fixed_size;
        unsigned int fixed_size_v[2];
    };

    int n_children;
    int _memory_size;
    struct TWidget **children;

    void *config; // possible additional config
    void *data;   // possible additional data

    void (*update)(struct TWidget *drawable);

    terminal_vector_t (*get_origin)(
        struct TWidget const *drawable);

    int (*draw_self)(struct TWidget const *drawable);
} twidget_t;

void init_twidget(twidget_t *drawable);

terminal_vector_t get_default_twidget_origin();

void update_twidget(twidget_t *drawable);

int draw_twidget(twidget_t *drawable);

void add_twidget_child(
    twidget_t *parent,
    twidget_t *child);

int twidget_child_index(
    twidget_t *parent,
    twidget_t *child);

void remove_twidget_child(
    twidget_t *parent,
    twidget_t *child,
    int release_child);

void free_twidget(twidget_t *drawable);

#endif
