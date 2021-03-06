#include "es_string.h"
#include "memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void es_string_init(es_string_t *string)
{
    string->size = 0;
    memset(string->_buffer, 0, sizeof(string->_buffer));
    string->_alloc = 0;
}

es_string_t es_string_create(const char *str)
{
    es_string_t string;
    es_string_init(&string);
    if (str && *str)
    {
        es_string_assign(&string, str);
    }
    return string;
}

void es_string_free(es_string_t *string)
{
    if (string->_alloc)
    {
        es_free(string->_heap_str.str);
    }
    es_string_init(string);
}

es_string_t *es_string_new(const char *str)
{
    void *string = es_malloc(sizeof(es_string_t));
    es_string_init(string);
    if (str && *str)
    {
        es_string_assign(string, str);
    }
    return string;
}

void es_string_delete(es_string_t *string)
{
    es_string_free(string);
    es_free(string);
}

char *es_string_get(es_string_t *string)
{
    return string->_alloc ? string->_heap_str.str : string->_buffer;
}

static void _alloc_str(es_string_t *string, es_size_t size)
{
    string->_heap_str.str = es_malloc(size + 1);
    string->_heap_str.memory_size = size + 1;
    string->_alloc = 1;
}

void es_string_resize(es_string_t *string, es_size_t size)
{
    if (string->_alloc || size >= ES_MAX_SSO_STRING_SIZE)
    {
        // alloc string on the heap
        if (string->_alloc && string->_heap_str.memory_size < size)
        {
            es_free(string->_heap_str.str);
            _alloc_str(string, size);
        }
        else if (!string->_alloc)
        {
            _alloc_str(string, size);
        }
        string->_alloc = 1;
    }
}

void es_string_assign(es_string_t *string, char const *str)
{
    const es_size_t size = strlen(str);
    es_string_assign_n(string, str, size);
}

void es_string_assign_n(
    es_string_t *string,
    char const *str,
    es_size_t size)
{
    es_string_clear(string);
    es_string_resize(string, size);
    strcpy(es_string_get(string), str);
    string->size = size;
}

void es_string_clear(es_string_t *string)
{
    *es_string_get(string) = 0;
    string->size = 0;
}

void es_string_concat(
    es_string_t *destination,
    es_string_t *left,
    es_string_t *right)
{
    es_string_resize(destination, left->size + right->size);
    char *dest = es_string_get(destination);
    strcpy(dest, es_string_get(left));
    strcpy(dest + left->size, es_string_get(right));
}

void es_string_print(es_string_t *string)
{
    printf(
        "str(alloc=%d): \"%s\"\n",
        (bool)string->_alloc,
        es_string_get(string));
}
