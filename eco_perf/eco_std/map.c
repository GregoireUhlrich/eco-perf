#include "map.h"
#include "error.h"
#include "memory.h"

static es_size_t _next_prime(es_size_t n);

void es_map_init(es_map_t *map, es_size_t size)
{
    map->size = 0;
    map->n_buckets = _next_prime(size);
    map->buckets = (es_map_item_t *)es_calloc(map->n_buckets, sizeof(es_map_item_t));
}

void es_map_free(es_map_t *map)
{
    es_map_clear(map);
    es_free(map->buckets);
}

void es_map_clear(es_map_t *map)
{
    memset(map->buckets, 0, map->n_buckets * sizeof(es_map_item_t));
    map->size = 0;
}

static void _insert_map_pair(
    es_map_t *map,
    es_map_key_t key,
    es_ref_t value)
{
    es_size_t index = key % map->n_buckets;
    es_size_t probe = 0;

    while (true)
    {
        if (!map->buckets[index].value)
        {
            map->size++;
            es_map_item_t *item = &map->buckets[index];
            item->key = key;
            item->probe = probe;
            item->value = value;
            return;
        }

        if (map->buckets[index].key == key)
        {
            map->buckets[index].value = value;
            return;
        }

        if (probe > map->buckets[index].probe)
        {
            es_map_item_t *item = &map->buckets[index];
            key = item->key;
            probe = item->probe;
            value = item->value;
            item->key = key;
            item->probe = probe;
            item->value = value;
        }
        index = (index + 1) % map->n_buckets;
        probe++;
    }
}

void es_map_resize(es_map_t *map, es_size_t size)
{
    es_size_t newSize = _next_prime(size);
    if (newSize <= map->n_buckets)
        return;

    es_map_item_t *oldBuckets = map->buckets;
    es_size_t oldSize = map->n_buckets;

    map->n_buckets = newSize;
    map->buckets = (es_map_item_t *)es_calloc(map->n_buckets, sizeof(es_map_item_t));
    map->size = 0;

    for (es_size_t i = 0; i < oldSize; i++)
    {
        if (!oldBuckets[i].value)
            continue;

        _insert_map_pair(map, oldBuckets[i].key, oldBuckets[i].value);
    }

    es_free(oldBuckets);
}

void es_map_put(es_map_t *map, es_map_key_t key, es_ref_t value)
{
    if (10 * map->size > 7 * map->n_buckets)
    {
        es_map_resize(map, 2 * map->n_buckets);
    }

    _insert_map_pair(map, key, value);
}

es_ref_t es_map_remove(es_map_t *map, es_map_key_t key)
{
    es_size_t index = key % map->n_buckets;
    es_size_t probe = 0;

    es_ref_t res = ES_NULL;

    while (map->buckets[index].value)
    {
        if (map->buckets[index].key == key)
        {
            res = map->buckets[index].value;
            es_size_t next = (index + 1) % map->n_buckets;

            while (map->buckets[next].value && map->buckets[next].probe > 0)
            {
                map->buckets[index] = map->buckets[next];
                map->buckets[index].probe -= 1;

                index = next;
                next = (index + 1) % map->n_buckets;
            }

            map->buckets[index].value = ES_NULL;
            map->size--;

            break;
        }

        if (map->buckets[index].probe < probe)
            break;

        index = (index + 1) % map->n_buckets;
        probe++;
    }

    if (8 * map->size < map->n_buckets)
        es_map_resize(map, map->n_buckets / 3);

    return res;
}

es_ref_t es_map_get(es_map_t *map, es_map_key_t key)
{
    es_size_t index = key % map->n_buckets;
    es_size_t probe = 0;
    es_ref_t res = ES_NULL;

    while (map->buckets[index].value)
    {
        if (map->buckets[index].key == key)
        {
            res = map->buckets[index].value;
            break;
        }

        if (map->buckets[index].probe < probe)
            break;

        index = ((index + 1) != map->n_buckets) ? (index + 1) : 0;
        probe++;
    }

    return res;
}

void es_map_for_each(es_map_t *map, es_map_pair_function_t f, es_ref_t user_data)
{
    for (es_size_t i = 0; i < map->n_buckets; i++)
    {
        es_map_item_t *walk = &map->buckets[i];
        if (walk->value)
            f(walk->key, walk->value, user_data);
    }
}

#if ES_USE_LONG_SIZE
static const es_size_t primes[] = {
    7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
    16381, 32749, 65521, 131071, 262139, 524287, 1048573,
    2097143, 4194301, 8388593, 16777213, 33554393,
    67108859, 134217689, 268435399, 536870909, 1073741789,
    2147483647, 4294967291, 8589934583, 17179869143,
    34359738337, 68719476731, 137438953447};
#else
static const es_size_t primes[] = {
    7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
    16381, 32749, 65521, 131071, 262139, 524287, 1048573,
    2097143, 4194301, 8388593, 16777213, 33554393,
    67108859, 134217689, 268435399, 536870909, 1073741789,
    2147483647, 4294967291};
#endif
static const es_size_t n_primes = sizeof(primes) / sizeof(es_size_t);

es_size_t _next_prime(es_size_t n)
{
    for (es_size_t i = 0; i < n_primes && primes[i] < ES_MAX_SIZE; i++)
    {
        if (n <= primes[i])
            return primes[i];
    }
    ES_ERROR(
        ES_OVERFLOW_ERROR,
        "Cannot build a map of size %d.",
        n)
}
