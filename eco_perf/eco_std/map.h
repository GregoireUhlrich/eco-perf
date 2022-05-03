#ifndef ECO_STD_MAP_H_INCLUDED
#define ECO_STD_MAP_H_INCLUDED

#include "def.h"
#include "hash.h"
#include <stdbool.h>

// Inspired from https://github.com/htop-dev/htop (HashTable.h/c)

typedef enum ESMapOwnership
{
    ES_MAP_NOT_OWNER = 0,
    ES_MAP_OWNS_KEY = 1 >> 1,
    ES_MAP_OWNS_VALUE = 1 >> 2,
    ES_MAP_OWNER = (ES_MAP_OWNS_KEY) | (ES_MAP_OWNS_VALUE)
} es_map_ownership_t;

typedef struct ESMapItem
{
    es_hash_t hash;
    es_size_t probe;
    es_ref_t key;
    es_ref_t value;
} es_map_item_t;

typedef struct ESMap
{
    es_size_t n_buckets;
    es_map_item_t *buckets;
    es_size_t size;
    es_hash_function_t hash;
    es_comparator_t key_eq;
    es_map_ownership_t ownership;
} es_map_t;

typedef void (*es_map_pair_function_t)(
    es_ref_t key,
    es_ref_t value,
    es_ref_t userdata);

void es_map_init(
    es_map_t *map,
    es_size_t size,
    es_map_ownership_t ownership,
    es_hash_function_t hash,
    es_comparator_t key_eq);

void es_map_free(es_map_t *map);

void es_map_clear(es_map_t *map);

void es_map_resize(es_map_t *map, es_size_t size);

void es_map_put(es_map_t *map, es_ref_t key, es_ref_t value);

es_ref_t es_map_remove(es_map_t *map, es_ref_t key);

es_ref_t es_map_get(es_map_t *map, es_ref_t key);

void es_map_for_each(
    es_map_t *map,
    es_map_pair_function_t f,
    es_ref_t userData);

#endif
