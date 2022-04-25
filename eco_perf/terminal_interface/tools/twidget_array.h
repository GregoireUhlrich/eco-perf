#ifndef ECO_PERF_TWIDGET_ARRAY_H_INCLUDED
#define ECO_PERF_TWIDGET_ARRAY_H_INCLUDED

struct TWidget;

typedef struct TWidgetArray
{
    unsigned int size;
    struct TWidget **widgets;
    unsigned int _memory_size;
} twidget_array_t;

void init_twidget_array(twidget_array_t *array);

void free_twidget_array(
    twidget_array_t *array);

void twidget_array_push_back(
    twidget_array_t *array,
    struct TWidget *widget);

int twidget_array_index_of(
    twidget_array_t *array,
    struct TWidget *widget);

void twidget_array_remove(
    twidget_array_t *array,
    struct TWidget *widget);

#endif
