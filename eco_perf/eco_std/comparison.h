#ifndef ECO_STD_COMPARISON_H_INCLUDED
#define ECO_STD_COMPARISON_H_INCLUDED

#include "def.h"
#include <stdbool.h>

// Built-in equality functions
bool es_char_eq(es_cref_t left, es_cref_t right);
bool es_uchar_eq(es_cref_t left, es_cref_t right);
bool es_short_eq(es_cref_t left, es_cref_t right);
bool es_ushort_eq(es_cref_t left, es_cref_t right);
bool es_int_eq(es_cref_t left, es_cref_t right);
bool es_uint_eq(es_cref_t left, es_cref_t right);
bool es_long_eq(es_cref_t left, es_cref_t right);
bool es_ulong_eq(es_cref_t left, es_cref_t right);

bool es_float_eq(es_cref_t left, es_cref_t right);
bool es_double_eq(es_cref_t left, es_cref_t right);

bool es_char_array_eq(es_cref_t left, es_cref_t right);
bool es_string_eq(es_cref_t left, es_cref_t right);

// Built-in comparison functions
bool es_char_comp(es_cref_t left, es_cref_t right);
bool es_uchar_comp(es_cref_t left, es_cref_t right);
bool es_short_comp(es_cref_t left, es_cref_t right);
bool es_ushort_comp(es_cref_t left, es_cref_t right);
bool es_int_comp(es_cref_t left, es_cref_t right);
bool es_uint_comp(es_cref_t left, es_cref_t right);
bool es_long_comp(es_cref_t left, es_cref_t right);
bool es_ulong_comp(es_cref_t left, es_cref_t right);

bool es_float_comp(es_cref_t left, es_cref_t right);
bool es_double_comp(es_cref_t left, es_cref_t right);

bool es_char_array_comp(es_cref_t left, es_cref_t right);
bool es_string_comp(es_cref_t left, es_cref_t right);

#endif
