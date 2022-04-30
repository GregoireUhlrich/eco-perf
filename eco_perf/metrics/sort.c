#include "sort.h"
#include <stdlib.h>

void ep_sort(
    void *data,
    size_t n_elements,
    size_t object_size,
    int (*comparison_func)(void const *, void const *))
{
    qsort(data, n_elements, object_size, comparison_func);
}

void *ep_bsearch(
    void *key,
    void *data,
    size_t n_elements,
    size_t object_size,
    int (*comparison_func)(void const *, void const *))
{
    return bsearch(key, data, n_elements, object_size, comparison_func);
}
