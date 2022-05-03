#include "hash.h"
#include "es_string.h"
#include <stdint.h>

#define DEFINE_DEFAULT_HASH(type, name)         \
    es_hash_t es_##name##_hash(es_cref_t key)   \
    {                                           \
        return (es_hash_t)(*(type const *)key); \
    }

DEFINE_DEFAULT_HASH(char, char)
DEFINE_DEFAULT_HASH(unsigned char, uchar)
DEFINE_DEFAULT_HASH(short, short)
DEFINE_DEFAULT_HASH(unsigned short, ushort)
DEFINE_DEFAULT_HASH(int, int)
DEFINE_DEFAULT_HASH(unsigned int, uint)
DEFINE_DEFAULT_HASH(long, long)
DEFINE_DEFAULT_HASH(unsigned long, ulong)

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
    char const *c_str = es_string_get((es_string_t *)str_ref);
#if ES_USE_LONG_SIZE
    return MurmurOAAT64(c_str);
#else
    return MurmurOAAT32(c_str);
#endif
}
