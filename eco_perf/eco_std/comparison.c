#include "comparison.h"
#include "es_string.h"
#include <string.h>

#define DEFINE_DEFAULT_COMP(type, name)                    \
    bool es_##name##_comp(es_cref_t left, es_cref_t right) \
    {                                                      \
        return *(type const *)left < *(type const *)right; \
    }

#define DEFINE_DEFAULT_EQ(type, name)                       \
    bool es_##name##_eq(es_cref_t left, es_cref_t right)    \
    {                                                       \
        return *(type const *)left == *(type const *)right; \
    }

#define DEFINE_DEFAULT_FUNCTIONS(type, name) \
    DEFINE_DEFAULT_EQ(type, name)            \
    DEFINE_DEFAULT_COMP(type, name)

DEFINE_DEFAULT_FUNCTIONS(char, char)
DEFINE_DEFAULT_FUNCTIONS(unsigned char, uchar)
DEFINE_DEFAULT_FUNCTIONS(short, short)
DEFINE_DEFAULT_FUNCTIONS(unsigned short, ushort)
DEFINE_DEFAULT_FUNCTIONS(int, int)
DEFINE_DEFAULT_FUNCTIONS(unsigned int, uint)
DEFINE_DEFAULT_FUNCTIONS(long, long)
DEFINE_DEFAULT_FUNCTIONS(unsigned long, ulong)
DEFINE_DEFAULT_FUNCTIONS(float, float)
DEFINE_DEFAULT_FUNCTIONS(double, double)

bool es_string_eq(es_cref_t left, es_cref_t right)
{
    return strcmp(
        es_string_get((es_string_t *)left),
        es_string_get((es_string_t *)right));
}

bool es_string_comp(es_cref_t left, es_cref_t right)
{
    return 0 > strcmp(
                   es_string_get((es_string_t *)left),
                   es_string_get((es_string_t *)right));
}
