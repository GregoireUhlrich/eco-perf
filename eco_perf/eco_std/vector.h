#ifndef ECO_STD_VECTOR_H_INCLUDED
#define ECO_STD_VECTOR_H_INCLUDED

#include "def.h"

typedef struct ESVector
{
    es_ref_t *data;
    es_size_t size;
    es_size_t _memory_size;
} es_vector_t;

struct ESContainer;

void es_vector_init(es_vector_t *vector);

void es_vector_init_from_container(
    es_vector_t *vector,
    struct ESContainer *container);

void es_vector_reserve(es_vector_t *vector, es_size_t size);

void es_vector_resize(es_vector_t *vector, es_size_t size);

void es_vector_clear(es_vector_t *vector);

void es_vector_free(es_vector_t *vector);

es_ref_t *es_vector_begin(const es_vector_t *vector);

es_ref_t *es_vector_end(const es_vector_t *vector);

void es_vector_push(es_vector_t *vector, es_ref_t ptr);

void es_vector_erase(es_vector_t *vector, es_size_t pos);

void es_vector_insert(
    es_vector_t *vector,
    es_size_t pos,
    es_ref_t value);

void es_vector_sort(
    es_vector_t *vector,
    es_comparator_t comp);

void es_vector_summary(es_vector_t const *vector);

#endif
