#ifndef ECO_STD_SET_H_INCLUDED
#define ECO_STD_SET_H_INCLUDED

#include "vector.h"

typedef struct ESSet
{
    es_vector_t _data;
    es_comparator_t comparator;
} es_set_t;

void es_set_init(es_set_t *set, es_comparator_t comparator);

void es_set_reserve(es_set_t *set, es_size_t size);

void es_set_free(es_set_t *set);

es_ref_t *es_set_begin(es_set_t *set);

es_ref_t *es_set_end(es_set_t *set);

es_ref_t *es_set_find(es_set_t *set, es_ref_t value_ref);

es_ref_t *es_set_insert(es_set_t *set, es_ref_t value_ref);

void es_set_erase(es_set_t *set, es_ref_t value_ref);

#endif
