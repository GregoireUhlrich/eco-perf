#include "container.h"
#include "error.h"
#include "memory.h"

#ifndef ES_ECO_MODE
#define _BOUND_CHECK(pos, size)                                   \
    {                                                             \
        ES_ASSERT(                                                \
            pos < size,                                           \
            ES_INDEX_ERROR,                                       \
            "Index %d is out of bounds of container of size %d.", \
            pos, size)                                            \
    }
#else
#define _BOUND_CHECK(pos, size)
#endif

void es_container_init(es_container_t *container, es_size_t object_size)
{
    ES_ASSERT(
        object_size > 0,
        ES_VALUE_ERROR,
        "Cannot create a container for object of size 0.")
    container->data = NULL;
    container->object_size = object_size;
    container->size = 0;
    container->_memory_size = 0;
}

void es_container_reserve(es_container_t *container, es_size_t size)
{
    ES_ASSERT(
        container->_memory_size == 0,
        ES_MEMORY_ERROR,
        "Cannot reserve space for non-empty container (%d blocks allocated)",
        container->_memory_size);
    container->data = es_malloc(size * container->object_size);
    container->_memory_size = size;
}

void es_container_clear(es_container_t *container)
{
    container->size = 0;
}

void es_container_free(es_container_t *container)
{
    es_free(container->data);
    es_container_init(container, container->object_size);
}

void es_container_push(es_container_t *container, void const *value_ptr)
{
    if (container->size == container->_memory_size)
    {
        container->data = es_realloc(
            container->data,
            2 * container->_memory_size * container->object_size);
        container->_memory_size *= 2;
    }
    memcpy(
        container->data + container->object_size * container->size,
        value_ptr,
        container->object_size);
    ++container->size;
}

void es_container_erase(es_container_t *container, es_size_t pos)
{
    const es_size_t size = container->size;
    const es_size_t object_size = container->object_size;
    _BOUND_CHECK(pos, size)
    void *first = &container[pos];
    void const *last = first + (size - pos) * object_size;
    while (first + object_size != last)
    {
        void *next = first + object_size;
        memcpy(first, next, object_size);
        first = next;
    }
    --container->size;
}

void *es_container_get(es_container_t *container, es_size_t pos)
{
    _BOUND_CHECK(pos, container->size)
    return (container->data + pos * (container->object_size));
}

void es_container_set(
    es_container_t *container,
    es_size_t pos,
    void const *value)
{
    _BOUND_CHECK(pos, container->size)
    memcpy(
        container->data + pos * (container->object_size),
        value,
        container->object_size);
}

es_iterator_t es_container_begin(es_container_t const *container)
{
    return container->data;
}

es_iterator_t es_container_end(es_container_t const *container)
{
    return container->data + container->size * container->object_size;
}

void es_container_incrementit(
    es_container_t const *container,
    es_iterator_t *iterator)
{
    *iterator += container->object_size;
}

void es_container_moveit(
    es_container_t const *container,
    es_iterator_t *iterator,
    es_size_t diff)
{
    *iterator += diff * container->object_size;
}

es_diff_t es_container_diffit(
    es_container_t const *container,
    es_iterator_t first,
    es_iterator_t last)
{
    return (last - first) / container->object_size;
}

void es_container_summary(es_container_t const *container)
{
    printf("container[sizeof %u](size = %u, memory = %u]\n",
           container->object_size,
           container->size,
           container->_memory_size);
}
