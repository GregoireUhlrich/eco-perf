#ifndef ECO_STD_DEF_H_INCLUDED
#define ECO_STD_DEF_H_INCLUDED

#include <stddef.h>

typedef void *es_ref_t;
typedef const void *es_cref_t;
typedef unsigned int es_size_t;
typedef int es_diff_t;

typedef void *es_iterator_t;

#define ES_POINTER_SIZE sizeof(void *)

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
