#include "container.h"
#include "error.h"
#include "memory.h"

void _bound_check(es_size_t pos, es_size_t size)
{
    ES_ASSERT(
        pos < size,
        ES_INDEX_ERROR,
        "Index %d is out of bounds of container of size %d.",
        pos, size)
}

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

void es_container_push(es_container_t *container, void *value_ptr)
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
    _bound_check(pos, size);
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

void es_container_summary(es_container_t const *const container)
{
    printf("container[sizeof %u](size = %u, memory = %u]\n",
           container->object_size,
           container->size,
           container->_memory_size);
}
