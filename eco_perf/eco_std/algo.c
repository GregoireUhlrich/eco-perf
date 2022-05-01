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

es_ref_t *_qsort_partition(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp);

void es_qsort(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp)
{
    if (last - first < 3) // 0 or 1 element, nothing to sort
    {
        return;
    }
    es_ref_t *pivot = _qsort_partition(first, last, comp);
    es_qsort(first, pivot, comp);
    es_qsort(pivot, last, comp);
}

es_ref_t *_qsort_partition(
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
