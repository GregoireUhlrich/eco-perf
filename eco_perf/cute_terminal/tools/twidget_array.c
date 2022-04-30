#include "twidget_array.h"
#include "../definitions/error.h"
#include "../widgets/twidget.h"
#include "memory.h"
#include <stdlib.h>

void init_twidget_array(twidget_array_t *array)
{
    array->size = 0;
    array->_memory_size = 0;
    array->widgets = NULL;
}

void allocate_twidget_array(twidget_array_t *array, int n_children)
{
    CT_ASSERT(array->size == 0,
              CT_RUNTIME_ERROR,
              "Cannot use the allocate children function on non-empty array.")
    if (n_children <= 0)
        return;
    array->widgets = (twidget_t **)malloc(n_children * sizeof(twidget_t *));
    array->_memory_size = n_children;
    array->size = n_children;
}

void resize_twidget_array(twidget_array_t *array, int n_children)
{
    if (array->size == n_children || n_children < 0)
        return;
    free_twidget_array(array);
    allocate_twidget_array(array, n_children);
}

void free_twidget_array(
    twidget_array_t *array)
{
    if (!array->widgets)
    {
        return;
    }
    free(array->widgets);
    array->widgets = NULL;
    array->size = 0;
    array->_memory_size = 0;
}

void twidget_array_push_back(
    twidget_array_t *array,
    struct TWidget *widget)
{
    if (array->size == array->_memory_size)
    {
        array->widgets = (twidget_t **)reallocate_array_memory(
            array->widgets,
            &array->_memory_size,
            sizeof(twidget_t *));
    }
    array->widgets[array->size] = widget;
    ++array->size;
}

int twidget_array_index_of(
    twidget_array_t *array,
    struct TWidget *widget)
{
    int index = -1;
    for (int i = 0; i != array->size; ++i)
    {
        if (array->widgets[i] == widget)
        {
            index = i;
            break;
        }
    }
    return index;
}

void twidget_array_remove(
    twidget_array_t *array,
    struct TWidget *widget)
{
    int index = twidget_array_index_of(array, widget);
    if (index == -1)
    {
        return;
    }
    for (int i = index; i != array->size - 1; ++i)
    {
        array->widgets[i] = array->widgets[i + 1];
    }
}
