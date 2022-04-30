#ifndef ECO_STD_MEMORY_H_INCLUDED
#define ECO_STD_MEMORY_H_INCLUDED

#include "def.h"

void *es_malloc(es_size_t size);

void es_free(void *ptr);

void *es_realloc(void *ptr, es_size_t size);

#endif
