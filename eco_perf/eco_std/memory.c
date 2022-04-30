#include "memory.h"
#include "error.h"
#include <stdlib.h>

void *es_malloc(es_size_t size)
{
    void *ptr = malloc(size);
    ES_ASSERT(ptr, ES_MEMORY_ERROR, "Bad allocation.")
    return ptr;
}

void es_free(void *ptr)
{
    ES_ASSERT(ptr, ES_MEMORY_ERROR, "Double free.")
    free(ptr);
}

void *es_realloc(void *ptr, es_size_t size)
{
    ptr = realloc(ptr, size);
    ES_ASSERT(ptr, ES_MEMORY_ERROR, "Bad allocation.")
    return ptr;
}
