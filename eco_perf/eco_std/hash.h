#ifndef ECO_STD_HASH_H_INCLUDED
#define ECO_STD_HASH_H_INCLUDED

#include "def.h"

typedef es_size_t es_hash_t;

typedef es_hash_t (*es_hash_function_t)(es_cref_t);

// Built-in hash functions
es_hash_t es_char_hash(es_cref_t key);
es_hash_t es_uchar_hash(es_cref_t key);
es_hash_t es_short_hash(es_cref_t key);
es_hash_t es_ushort_hash(es_cref_t key);
es_hash_t es_int_hash(es_cref_t key);
es_hash_t es_uint_hash(es_cref_t key);
es_hash_t es_long_hash(es_cref_t key);
es_hash_t es_ulong_hash(es_cref_t key);

es_hash_t es_string_hash(es_cref_t key);

#endif
