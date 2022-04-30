#ifndef ECO_STD_VECTOR_H_INCLUDED
#define ECO_STD_VECTOR_H_INCLUDED

#include "def.h"

typedef struct ESVector
{
    es_object_t *data;
    es_size_t object_size;
    es_size_t size;
    es_size_t _memory_size;
} es_vector_t;

void init_es_vector(es_vector_t *vector, es_size_t object_size);

void es_vector_push(es_vector_t *vector, es_object_t data);

void es_vector_reserve(es_vector_t *vector, es_size_t size);

void clear_es_vector(es_vector_t *vector);

void free_es_vector(es_vector_t *vector);

#endif
