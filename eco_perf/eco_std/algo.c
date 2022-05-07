#include "algo.h"

es_ref_t *es_bsearch(
    es_ref_t *first,
    es_ref_t *last,
    es_ref_t value_ref,
    es_comparator_t comp)
{
    if (first == last)
    {
        return first;
    }
    --last;
    es_diff_t diff = last - first;
    if (diff == 0)
    {
        return comp(value_ref, *first) ? first : first + 1;
    }
    while (diff > 1)
    {
        es_ref_t *middle = first + diff / 2;
        if (comp(value_ref, *middle))
        {
            last = middle;
        }
        else
        {
            first = middle;
        }
        diff = last - first;
    }
    if (comp(value_ref, *first))
        return first;
    if (comp(value_ref, *last))
        return last;
    return last + 1;
}

es_size_t es_bsearch_find(
    es_ref_t *first,
    es_ref_t *last,
    es_ref_t value_ref,
    es_comparator_t comp)
{
    es_ref_t *pos = es_bsearch(first, last, value_ref, comp);
    if (pos == first)
    {
        return ES_NPOS;
    }
    return comp(*(pos - 1), value_ref) ? ES_NPOS : (pos - first - 1);
}

void es_isort(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp)
{
    if (last - first < 2)
    {
        return;
    }
    es_ref_t *i = first + 1;
    while (i != last)
    {
        const es_ref_t x = *i;
        es_ref_t *j = i - 1;
        while (j >= first && comp(x, *j))
        {
            *(j + 1) = *j;
            --j;
        }
        *(j + 1) = x;
        ++i;
    }
}

void es_partial_isort(
    es_ref_t *first,
    es_ref_t *last,
    int n_sorted,
    es_comparator_t comp)
{
    if (first == last)
    {
        return;
    }
    es_ref_t *i = first + 1;
    int n_current_sorted = 0;
    while (i != last && n_current_sorted++ < n_sorted)
    {
        const es_ref_t x = *i;
        es_ref_t *j = i - 1;
        while (j >= first && comp(x, *j))
        {
            *(j + 1) = *j;
            --j;
        }
        *(j + 1) = x;
        ++i;
    }
}

#define USE_HOARE_PARTITION 1
#define THRESHOLD_INSERTION_SORT 10

#if USE_HOARE_PARTITION == 0
static es_ref_t *_qsort_lomuto_partition(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp);
#else
static es_ref_t *_qsort_hoare_partition(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp);
#endif

void es_qsort(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp)
{
    if (last - first < 2) // 0 or 1 element, nothing to sort
    {
        return;
    }
    if (last - first < THRESHOLD_INSERTION_SORT)
    {
        return es_isort(first, last, comp);
    }
#if USE_HOARE_PARTITION == 1
    es_ref_t *pivot = _qsort_hoare_partition(first, last, comp);
    es_qsort(first, pivot + 1, comp);
    es_qsort(pivot + 1, last, comp);
#else
    es_ref_t *pivot = _qsort_lomuto_partition(first, last, comp);
    es_qsort(first, pivot, comp);
    es_qsort(pivot + 1, last, comp);
#endif
}

void es_partial_qsort(
    es_ref_t *first,
    es_ref_t *last,
    int n_sorted,
    es_comparator_t comp)
{
    if (last - first < 3 || n_sorted <= 0) // 0 or 1 element, nothing to sort
    {
        return;
    }
    if (last - first < THRESHOLD_INSERTION_SORT)
    {
        return es_partial_isort(first, last, n_sorted, comp);
    }
#if USE_HOARE_PARTITION == 1
    es_ref_t *pivot = _qsort_hoare_partition(first, last, comp);
    es_partial_qsort(first, pivot + 1, n_sorted, comp);
    if (pivot - first <= n_sorted)
    {
        es_partial_qsort(pivot + 1, last, n_sorted - (pivot - first), comp);
    }
#else
    es_ref_t *pivot = _qsort_lomuto_partition(first, last, comp);
    es_partial_qsort(first, pivot, n_sorted, comp);
    if (pivot - first <= n_sorted)
    {
        es_partial_qsort(pivot + 1, last, n_sorted - (pivot - first), comp);
    }
#endif
}

// Source: https://en.wikipedia.org/wiki/Quicksort

#if USE_HOARE_PARTITION == 0

es_ref_t *_qsort_lomuto_partition(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp)
{
    --last; // put last on a valid value
    const es_ref_t pivot = *last;
    es_ref_t *pivot_pos = first - 1;
    for (es_ref_t *iter = first; iter != last; ++iter)
    {
        if (comp(*iter, pivot))
        {
            ++pivot_pos;
            const es_ref_t foo = *pivot_pos;
            *pivot_pos = *iter;
            *iter = foo;
        }
    }
    ++pivot_pos;
    *last = *pivot_pos;
    *pivot_pos = pivot;

    return pivot_pos;
}

#else

es_ref_t *_qsort_hoare_partition(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp)
{
    --last; // put last on a valid value
    const es_diff_t diff = last - first;
    es_ref_t *middle = first + diff / 2;
    es_ref_t pivot = *middle;
    es_ref_t *i = first - 1;
    es_ref_t *j = last + 1;
    while (true)
    {
        do
        {
            ++i;
        } while (comp(*i, pivot));
        do
        {
            --j;
        } while (comp(pivot, *j));
        if (i >= j)
            return j;
        const es_ref_t foo = *i;
        *i = *j;
        *j = foo;
    }
}

#endif
