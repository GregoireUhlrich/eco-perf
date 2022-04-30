#include "vector.h"
#include "error.h"
#include "memory.h"

void _bound_check(es_size_t pos, es_size_t size)
{
    ES_ASSERT(
        pos < size,
        ES_INDEX_ERROR,
        "Index %d is out of bounds of vector of size %d.",
        pos, size)
}

void es_vector_init(es_vector_t *vector, es_size_t object_size)
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

void es_vector_clear(es_vector_t *vector)
{
    vector->size = 0;
}

void es_vector_free(es_vector_t *vector)
{
    es_free(vector->data);
    es_vector_init(vector, vector->object_size);
}

void es_vector_push(es_vector_t *vector, void *value_ptr)
{
    if (vector->size == vector->_memory_size)
    {
        vector->data = es_realloc(
            vector->data,
            2 * vector->_memory_size * vector->object_size);
        vector->_memory_size *= 2;
    }
    memcpy(
        vector->data + vector->object_size * vector->size,
        value_ptr,
        vector->object_size);
    ++vector->size;
}

void es_vector_erase(es_vector_t *vector, es_size_t pos)
{
    const es_size_t size = vector->size;
    const es_size_t object_size = vector->object_size;
    _bound_check(pos, size);
    void *first = &vector[pos];
    void const *last = first + (size - pos) * object_size;
    while (first + object_size != last)
    {
        void *next = first + object_size;
        memcpy(first, next, object_size);
        first = next;
    }
    --vector->size;
}

void es_vector_summary(es_vector_t const *const vector)
{
    printf("vector[sizeof %u](size = %u, memory = %u]\n",
           vector->object_size,
           vector->size,
           vector->_memory_size);
}
