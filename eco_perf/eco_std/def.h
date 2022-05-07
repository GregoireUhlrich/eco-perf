#ifndef ECO_STD_DEF_H_INCLUDED
#define ECO_STD_DEF_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef ES_USE_LONG_SIZE
#define ES_USE_LONG_SIZE false
#endif

typedef void *es_ref_t;
typedef const void *es_cref_t;

#if ES_USE_LONG_SIZE
typedef unsigned long es_size_t;
typedef long es_diff_t;
#else
typedef unsigned int es_size_t;
typedef int es_diff_t;
#endif

#define ES_NULL NULL
#define ES_POINTER_SIZE sizeof(void *)
#define ES_NPOS ((es_size_t)(-1))

#define ES_MAX_SIZE ((SIZE_MAX < ES_NPOS) ? SIZE_MAX : ES_NPOS)

typedef void *es_iterator_t;

typedef bool (*es_comparator_t)(es_cref_t, es_cref_t);

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

#endif
