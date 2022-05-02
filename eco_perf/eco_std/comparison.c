#include "comparison.h"
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
    return strcmp(*(char const **)left, *(char const **)right) == 0;
}

bool es_string_comp(es_cref_t left, es_cref_t right)
{
    return strcmp(*(char const **)left, *(char const **)right) < 0;
}

bool es_char_array_eq(es_cref_t left, es_cref_t right)
{
    return strcmp((char const *)left, (char const *)right) == 0;
}

bool es_char_array_comp(es_cref_t left, es_cref_t right)
{
    return strcmp((char const *)left, (char const *)right) < 0;
}
