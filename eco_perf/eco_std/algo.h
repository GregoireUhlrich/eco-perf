#ifndef ECO_STD_ALGO_H_INCLUDED
#define ECO_STD_ALGO_H_INCLUDED

#include "def.h"

es_ref_t *es_bsearch(
    es_ref_t *first,
    es_ref_t *last,
    es_ref_t value_ref,
    es_comparator_t comp);

es_size_t es_bsearch_find(
    es_ref_t *first,
    es_ref_t *last,
    es_ref_t value_ref,
    es_comparator_t comp);

void es_qsort(
    es_ref_t *first,
    es_ref_t *last,
    es_comparator_t comp);

#endif
