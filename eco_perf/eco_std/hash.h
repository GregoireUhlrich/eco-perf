#ifndef ECO_STD_HASH_H_INCLUDED
#define ECO_STD_HASH_H_INCLUDED

#include "def.h"
#include <stdbool.h>

typedef es_size_t es_hash_t;

typedef es_hash_t (*es_hash_function_t)(es_cref_t);

// Built-in has functions
es_hash_t es_char_hash(es_cref_t key);
es_hash_t es_uchar_hash(es_cref_t key);
es_hash_t es_short_hash(es_cref_t key);
es_hash_t es_ushort_hash(es_cref_t key);
es_hash_t es_int_hash(es_cref_t key);
es_hash_t es_uint_hash(es_cref_t key);
es_hash_t es_long_hash(es_cref_t key);
es_hash_t es_ulong_hash(es_cref_t key);

es_hash_t es_string_hash(es_cref_t key);

// Built-in comparison functions
bool es_char_comp(es_cref_t left, es_cref_t right);
bool es_uchar_comp(es_cref_t left, es_cref_t right);
bool es_short_comp(es_cref_t left, es_cref_t right);
bool es_ushort_comp(es_cref_t left, es_cref_t right);
bool es_int_comp(es_cref_t left, es_cref_t right);
bool es_uint_comp(es_cref_t left, es_cref_t right);
bool es_long_comp(es_cref_t left, es_cref_t right);
bool es_ulong_comp(es_cref_t left, es_cref_t right);

bool es_string_comp(es_cref_t left, es_cref_t right);

#endif
