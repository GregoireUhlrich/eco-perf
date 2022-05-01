#ifndef ECO_STD_CONTAINER_H_INCLUDED
#define ECO_STD_CONTAINER_H_INCLUDED

#include "def.h"
typedef struct ESContainer
{
    void *data;
    es_size_t object_size;
    es_size_t size;
    es_size_t _memory_size;
} es_container_t;

void es_container_init(es_container_t *container, es_size_t object_size);

void es_container_reserve(es_container_t *container, es_size_t size);

void es_container_clear(es_container_t *container);

void es_container_free(es_container_t *container);

void es_container_push(es_container_t *container, es_cref_t value_ptr);

void es_container_erase(es_container_t *container, es_size_t pos);

es_ref_t es_container_get(es_container_t *container, es_size_t pos);

void es_container_set(
    es_container_t *container,
    es_size_t pos,
    es_cref_t value);

es_iterator_t es_container_begin(es_container_t const *container);

es_iterator_t es_container_end(es_container_t const *container);

void es_container_incrementit(
    es_container_t const *container,
    es_iterator_t *iterator);

void es_container_moveit(
    es_container_t const *container,
    es_iterator_t *iterator,
    es_size_t diff);

es_diff_t es_container_diffit(
    es_container_t const *container,
    es_iterator_t first,
    es_iterator_t last);

void es_container_summary(es_container_t const *container);

#endif
