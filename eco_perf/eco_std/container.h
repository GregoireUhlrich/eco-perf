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

void es_container_push(es_container_t *container, void *value_ptr);

void es_container_erase(es_container_t *container, es_size_t pos);

void es_container_summary(es_container_t const *const container);

#endif
