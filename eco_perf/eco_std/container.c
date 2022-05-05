#include "container.h"
#include "error.h"
#include "memory.h"

void es_container_init(es_container_t *container, es_size_t object_size)
{
    ES_ASSERT(
        object_size > 0,
        ES_VALUE_ERROR,
        "Cannot create a container for object of size 0.")
    container->data = ES_NULL;
    container->object_size = object_size;
    container->size = 0;
    container->_memory_size = 0;
}

es_container_t es_container_create(es_size_t object_size)
{
    es_container_t container;
    es_container_init(&container, object_size);
    return container;
}

void es_container_free(es_container_t *container)
{
    es_free(container->data);
    es_container_init(container, container->object_size);
}

es_container_t *es_container_new(es_size_t object_size)
{
    void *container = es_malloc(sizeof(es_container_t));
    es_container_init(container, object_size);
    return container;
}

void es_container_delete(es_container_t *container)
{
    es_container_free(container);
    es_free(container);
}

void _es_container_alloc(es_container_t *container)
{
    if (container->_memory_size == 0)
    {
        container->data = es_malloc(container->object_size);
        container->_memory_size = 1;
    }
    else
    {
        container->data = es_realloc(
            container->data,
            2 * container->_memory_size * container->object_size);
        container->_memory_size *= 2;
    }
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

bool es_container_push(
    es_container_t *container,
    es_cref_t value_ptr)
{
    bool alloc = false;
    if (container->size == container->_memory_size)
    {
        _es_container_alloc(container);
        alloc = true;
    }
    if (value_ptr)
    {
        memcpy(
            container->data + container->object_size * container->size,
            value_ptr,
            container->object_size);
    }
    else
    {
        memset(
            container->data + container->object_size * container->size,
            0,
            container->object_size);
    }
    ++container->size;
    return alloc;
}

void es_container_erase(es_container_t *container, es_size_t pos)
{
    const es_size_t size = container->size;
    const es_size_t object_size = container->object_size;
    _BOUND_CHECK(pos, size)
    es_iterator_t first = es_container_begin(container);
    es_container_moveit(container, &first, pos);
    const es_iterator_t last = es_container_end(container);
    while (first + object_size != last)
    {
        void *next = first + object_size;
        memcpy(first, next, object_size);
        first = next;
    }
    --container->size;
}

es_ref_t es_container_get(es_container_t *container, es_size_t pos)
{
    _BOUND_CHECK(pos, container->size)
    return (container->data + pos * (container->object_size));
}

void es_container_set(
    es_container_t *container,
    es_size_t pos,
    es_cref_t value)
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
