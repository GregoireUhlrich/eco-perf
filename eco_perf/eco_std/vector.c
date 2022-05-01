#include "vector.h"
#include "error.h"
#include "memory.h"

void es_vector_init(es_vector_t *vector)
{
    vector->data = ES_NULL;
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
    vector->data = es_malloc(size * ES_POINTER_SIZE);
    vector->_memory_size = size;
}

void es_vector_resize(es_vector_t *vector, es_size_t size)
{
    if (size <= vector->_memory_size)
    {
        vector->size = size;
        return;
    }
    if (vector->_memory_size == 0)
    {
        vector->data = es_malloc(size * ES_POINTER_SIZE);
    }
    else
    {
        vector->data = es_realloc(vector->data, size * ES_POINTER_SIZE);
    }
    vector->size = size;
    vector->_memory_size = size;
}

void es_vector_clear(es_vector_t *vector)
{
    vector->size = 0;
}

void es_vector_free(es_vector_t *vector)
{
    es_free(vector->data);
    es_vector_init(vector);
}

es_ref_t *es_vector_begin(const es_vector_t *vector)
{
    return vector->data;
}

es_ref_t *es_vector_end(const es_vector_t *vector)
{
    return vector->data + vector->size;
}

void _es_vector_alloc(es_vector_t *vector)
{
    if (vector->_memory_size == 0)
    {
        vector->data = es_malloc(ES_POINTER_SIZE);
        vector->_memory_size = 1;
    }
    else
    {
        vector->data = es_realloc(
            vector->data,
            2 * vector->_memory_size * ES_POINTER_SIZE);
        vector->_memory_size *= 2;
    }
}

void es_vector_push(es_vector_t *vector, es_ref_t ptr)
{
    if (vector->size == vector->_memory_size)
    {
        _es_vector_alloc(vector);
    }
    vector->data[vector->size] = ptr;
    ++vector->size;
}

void es_vector_erase(es_vector_t *vector, es_size_t pos)
{
    const es_size_t size = vector->size;
    _BOUND_CHECK(pos, size)
    void **first = &vector->data[pos];
    void **last = first + (size - pos);
    while (first + 1 != last)
    {
        void **next = first + 1;
        *first = *next;
        first = next;
    }
    --vector->size;
}

void es_vector_insert(
    es_vector_t *vector,
    es_size_t pos,
    es_ref_t value)
{
    _BOUND_CHECK(pos, vector->size)
    es_vector_push(vector, ES_NULL);
    const es_size_t last = vector->size - 1;
    for (int i = last; i != pos; --i)
    {
        vector->data[i] = vector->data[i - 1];
    }
    vector->data[pos] = value;
}

void es_vector_summary(es_vector_t const *vector)
{
    printf("vector[sizeof %lu](size = %u, memory = %u]\n",
           ES_POINTER_SIZE,
           vector->size,
           vector->_memory_size);
}
