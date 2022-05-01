#ifndef ECO_STD_MAP_H_INCLUDED
#define ECO_STD_MAP_H_INCLUDED

#include "def.h"
#include <stdbool.h>

typedef unsigned int es_map_key_t;

typedef struct ESMapItem
{
    es_map_key_t key;
    es_size_t probe;
    es_ref_t value;
} es_map_item_t;

typedef struct ESMap
{
    es_size_t size;
    es_map_item_t *buckets;
    es_size_t items;
} es_map_t;

typedef void (*es_map_pair_function_t)(
    es_map_key_t key,
    void *value,
    void *userdata);

void es_map_init(es_map_t *map, es_size_t size);

void es_map_free(es_map_t *map);

void es_map_clear(es_map_t *map);

void es_map_set_size(es_map_t *map, es_size_t size);

void es_map_put(es_map_t *map, es_map_key_t key, void *value);

void *es_map_remove(es_map_t *map, es_map_key_t key);

void *es_map_get(es_map_t *map, es_map_key_t key);

void es_map_for_each(
    es_map_t *map,
    es_map_pair_function_t f,
    void *userData);

#endif
