#include "map.h"

/*
static const uint64_t OEISprimes[] = {
    7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
    16381, 32749, 65521, 131071, 262139, 524287, 1048573,
    2097143, 4194301, 8388593, 16777213, 33554393,
    67108859, 134217689, 268435399, 536870909, 1073741789,
    2147483647, 4294967291, 8589934583, 17179869143,
    34359738337, 68719476731, 137438953447};

static size_t nextPrime(size_t n)
{
    for (size_t i = 0; i < ARRAYSIZE(OEISprimes) && OEISprimes[i] < SIZE_MAX; i++)
    {
        if (n <= OEISprimes[i])
            return OEISprimes[i];
    }

    CRT_fatalError("Hashtable: no prime found");
}

es_map_t *es_map_init(size_t size, bool owner)
{
    es_map_t *map;

    map = xMalloc(sizeof(es_map_t));
    map->items = 0;
    map->size = size ? nextPrime(size) : 13;
    map->buckets = (es_map_item_t *)xCalloc(map->size, sizeof(es_map_item_t));
    map->owner = owner;

    assert(Hashtable_isConsistent(map));
    return map;
}

void es_map_free(es_map_t *map)
{
    es_map_clear(map);

    free(map->buckets);
    free(map);
}

void es_map_clear(es_map_t *map)
{
    assert(Hashtable_isConsistent(map));

    if (map->owner)
        for (size_t i = 0; i < map->size; i++)
            free(map->buckets[i].value);

    memset(map->buckets, 0, map->size * sizeof(es_map_item_t));
    map->items = 0;

    assert(Hashtable_isConsistent(map));
}

static void insert(es_map_t *map, es_map_key_t key, void *value)
{
    size_t index = key % map->size;
    size_t probe = 0;

    for (;;)
    {
        if (!map->buckets[index].value)
        {
            map->items++;
            map->buckets[index].key = key;
            map->buckets[index].probe = probe;
            map->buckets[index].value = value;
            return;
        }

        if (map->buckets[index].key == key)
        {
            if (map->owner && map->buckets[index].value != value)
                free(map->buckets[index].value);
            map->buckets[index].value = value;
            return;
        }

        if (probe > map->buckets[index].probe)
        {
            es_map_item_t tmp = map->buckets[index];

            map->buckets[index].key = key;
            map->buckets[index].probe = probe;
            map->buckets[index].value = value;

            key = tmp.key;
            probe = tmp.probe;
            value = tmp.value;
        }

        index = (index + 1) % map->size;
        probe++;

        assert(index != origIndex);
    }
}

void es_map_set_size(es_map_t *map, size_t size)
{

    if (size <= map->items)
        return;

    size_t newSize = nextPrime(size);
    if (newSize == map->size)
        return;

    es_map_item_t *oldBuckets = map->buckets;
    size_t oldSize = map->size;

    map->size = newSize;
    map->buckets = (es_map_item_t *)xCalloc(map->size, sizeof(es_map_item_t));
    map->items = 0;

    for (size_t i = 0; i < oldSize; i++)
    {
        if (!oldBuckets[i].value)
            continue;

        insert(map, oldBuckets[i].key, oldBuckets[i].value);
    }

    free(oldBuckets);

    assert(Hashtable_isConsistent(map));
}

void es_map_put(es_map_t *map, es_map_key_t key, void *value)
{

    assert(Hashtable_isConsistent(map));
    assert(map->size > 0);
    assert(value);

    if (10 * map->items > 7 * map->size)
    {
        if (SIZE_MAX / 2 < map->size)
            CRT_fatalError("Hashtable: size overflow");

        es_map_set_size(map, 2 * map->size);
    }

    insert(map, key, value);

    assert(Hashtable_isConsistent(map));
    assert(es_map_get(map, key) != NULL);
    assert(map->size > map->items);
}

void *es_map_remove(es_map_t *map, es_map_key_t key)
{
    size_t index = key % map->size;
    size_t probe = 0;

    assert(Hashtable_isConsistent(map));

    void *res = NULL;

    while (map->buckets[index].value)
    {
        if (map->buckets[index].key == key)
        {
            if (map->owner)
            {
                free(map->buckets[index].value);
            }
            else
            {
                res = map->buckets[index].value;
            }

            size_t next = (index + 1) % map->size;

            while (map->buckets[next].value && map->buckets[next].probe > 0)
            {
                map->buckets[index] = map->buckets[next];
                map->buckets[index].probe -= 1;

                index = next;
                next = (index + 1) % map->size;
            }

            map->buckets[index].value = NULL;
            map->items--;

            break;
        }

        if (map->buckets[index].probe < probe)
            break;

        index = (index + 1) % map->size;
        probe++;

        assert(index != origIndex);
    }

    assert(Hashtable_isConsistent(map));
    assert(es_map_get(map, key) == NULL);

    if (8 * map->items < map->size)
        es_map_set_size(map, map->size / 3);

    return res;
}

void *es_map_get(es_map_t *map, es_map_key_t key)
{
    size_t index = key % map->size;
    size_t probe = 0;
    void *res = NULL;

    assert(Hashtable_isConsistent(map));

    while (map->buckets[index].value)
    {
        if (map->buckets[index].key == key)
        {
            res = map->buckets[index].value;
            break;
        }

        if (map->buckets[index].probe < probe)
            break;

        index = (index + 1) != map->size ? (index + 1) : 0;
        probe++;

        assert(index != origIndex);
    }

    return res;
}

void es_map_for_each(es_map_t *map, es_map_pair_function_t f, void *userData)
{
    assert(Hashtable_isConsistent(map));
    for (size_t i = 0; i < map->size; i++)
    {
        es_map_item_t *walk = &map->buckets[i];
        if (walk->value)
            f(walk->key, walk->value, userData);
    }
    assert(Hashtable_isConsistent(map));
}
*/
