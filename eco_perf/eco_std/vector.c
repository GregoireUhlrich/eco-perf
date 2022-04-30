#include "vector.h"
#include "error.h"
#include "memory.h"

void init_es_vector(es_vector_t *vector, es_size_t object_size)
{
    ES_ASSERT(
        object_size > 0,
        ES_VALUE_ERROR,
        "Cannot create a vector for object of size 0.")
    vector->data = NULL;
    vector->object_size = object_size;
    vector->size = 0;
    vector->_memory_size = 0;
}

void es_vector_push(es_vector_t *vector, es_object_t data)
{
    if (vector->size == vector->_memory_size)
    {
        vector->data = es_realloc(
            vector->data,
            2 * vector->_memory_size * vector->object_size);
        vector->_memory_size *= 2;
    }
    vector->data[vector->size] = data;
    ++vector->size;
}

void es_vector_reserve(es_vector_t *vector, es_size_t size)
{
    ES_ASSERT(
        vector->_memory_size == 0,
        ES_MEMORY_ERROR,
        "Cannot reserve space for non-empty vector (%d blocks allocated)",
        vector->_memory_size);
    vector->data = es_malloc(size * vector->object_size);
    vector->_memory_size = size;
}

void clear_es_vector(es_vector_t *vector)
{
    vector->size = 0;
}

void free_es_vector(es_vector_t *vector)
{
    es_free(vector->data);
    init_es_vector(vector, vector->object_size);
}
