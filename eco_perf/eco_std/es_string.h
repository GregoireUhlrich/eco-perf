#ifndef ECO_STD_STRING_H_INCLUDED
#define ECO_STD_STRING_H_INCLUDED

#include "def.h"
#include <stdbool.h>

#if ES_USE_LONG_SIZE
#define ES_MAX_SSO_STRING_SIZE ES_POINTER_SIZE + 8
#else
#define ES_MAX_SSO_STRING_SIZE ES_POINTER_SIZE + 4
#endif

typedef struct _ESHeapString
{
    char *str;
    es_size_t memory_size;
} _es_heap_string_t;

typedef struct ESString
{
    es_size_t size;
    union
    {
        _es_heap_string_t _heap_str;
        char _buffer[ES_MAX_SSO_STRING_SIZE];
    };
    char _alloc;
} es_string_t;

void es_string_init(es_string_t *string);

es_string_t es_string_create();

void es_string_free(es_string_t *string);

es_string_t *es_string_create_new();

void es_string_delete(es_string_t *string);

char *es_string_get(es_string_t *string);

void es_string_resize(es_string_t *string, es_size_t size);

void es_string_assign(es_string_t *string, char const *str);

void es_string_assign_n(
    es_string_t *string,
    char const *str,
    es_size_t size);

void es_string_clear(es_string_t *string);

void es_string_concat(
    es_string_t *destination,
    es_string_t *left,
    es_string_t *right);

void es_string_print(es_string_t *string);

#endif
