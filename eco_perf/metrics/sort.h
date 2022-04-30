#ifndef ECO_PERF_SORT_H_INCLUDED
#define ECO_PERF_SORT_H_INCLUDED

#include <stddef.h>

void ep_sort(
    void *data,
    size_t n_elements,
    size_t object_size,
    int (*comparison_func)(void const *, void const *));

void *ep_bsearch(
    void *key,
    void *data,
    size_t n_elements,
    size_t object_size,
    int (*comparison_func)(void const *, void const *));

#endif
