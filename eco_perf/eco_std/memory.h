#ifndef ECO_STD_MEMORY_H_INCLUDED
#define ECO_STD_MEMORY_H_INCLUDED

#include "def.h"

void *es_malloc(es_size_t size);

void *es_calloc(es_size_t num, es_size_t size);

void *es_realloc(void *ptr, es_size_t size);

void es_free(void *ptr);

#endif
