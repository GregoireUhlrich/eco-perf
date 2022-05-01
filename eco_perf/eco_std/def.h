#ifndef ECO_STD_DEF_H_INCLUDED
#define ECO_STD_DEF_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

typedef void *es_ref_t;
typedef const void *es_cref_t;
typedef unsigned int es_size_t;
typedef int es_diff_t;

#define ES_NULL NULL
#define ES_POINTER_SIZE sizeof(void *)
#define ES_NPOS ((es_size_t)(-1))

typedef void *es_iterator_t;

typedef bool (*es_comparator_t)(es_cref_t, es_cref_t);

#define ES_DEFINE_DEFAULT_COMP(type, func_name)     \
    bool func_name(es_cref_t A, es_cref_t B)        \
    {                                               \
        return *(const type *)A < *(const type *)B; \
    }

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
