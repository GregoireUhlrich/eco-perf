#ifndef ECO_PERF_TERM_DRAWABLE_H_INCLUDED
#define ECO_PERF_TERM_DRAWABLE_H_INCLUDED

#include "../terminal/vector.h"
#include "../tools/twidget_array.h"
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

#endif
