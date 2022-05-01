#include "hash.h"
#include <stdint.h>
#include <string.h>

#define DEFINE_DEFAULT_HASH(type, name)         \
    es_hash_t es_##name##_hash(es_cref_t key)   \
    {                                           \
        return (es_hash_t)(*(type const *)key); \
    }

#define DEFINE_DEFAULT_COMP(type, name)                     \
    bool es_##name##_comp(es_cref_t left, es_cref_t right)  \
    {                                                       \
        return *(type const *)left == *(type const *)right; \
    }

#define DEFINE_DEFAULT_FUNCTIONS(type, name) \
    DEFINE_DEFAULT_HASH(type, name)          \
    DEFINE_DEFAULT_COMP(type, name)

DEFINE_DEFAULT_FUNCTIONS(char, char)
DEFINE_DEFAULT_FUNCTIONS(unsigned char, uchar)
DEFINE_DEFAULT_FUNCTIONS(short, short)
DEFINE_DEFAULT_FUNCTIONS(unsigned short, ushort)
DEFINE_DEFAULT_FUNCTIONS(int, int)
DEFINE_DEFAULT_FUNCTIONS(unsigned int, uint)
DEFINE_DEFAULT_FUNCTIONS(long, long)
DEFINE_DEFAULT_FUNCTIONS(unsigned long, ulong)

// Source https://stackoverflow.com/questions/7666509/hash-function-for-string
static uint32_t MurmurOAAT32(const char *key)
{
    uint32_t h = 3323198485;
    for (; *key; ++key)
    {
        h ^= *key;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

// Source https://stackoverflow.com/questions/7666509/hash-function-for-string
static uint64_t MurmurOAAT64(const char *key)
{
    uint64_t h = 525201411107845655;
    for (; *key; ++key)
    {
        h ^= *key;
        h *= 0x5bd1e9955bd1e995;
        h ^= h >> 47;
    }
    return h;
}

es_hash_t es_string_hash(es_cref_t str_ref)
{
    char const *str = *(char const **)str_ref;
#if ES_USE_LONG_SIZE
    return MurmurOAAT64(str);
#else
    return MurmurOAAT32(str);
#endif
}

bool es_string_comp(es_cref_t left, es_cref_t right)
{
    return strcmp(*(char const **)left, *(char const **)right) == 0;
}
