#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/comparison.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/es_string.h"
#include "eco_perf/eco_std/map.h"
#include "eco_perf/eco_std/vector.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

bool weird_int_order(es_cref_t A, es_cref_t B)
{
    const int a = *(int const *)A;
    const int b = *(int const *)B;
    if (a == 4)
        return false;
    if ((a & 1) != (b & 1))
    {
        return a & 1;
    }
    return a > b;
}

int main()
{
    es_container_t int_container;
    es_container_init(&int_container, sizeof(int));
    es_container_summary(&int_container);
    es_container_reserve(&int_container, 4);
    es_container_summary(&int_container);
    int array[] = {1, 2, 3};
    es_container_push(&int_container, &array[0]);
    es_container_push(&int_container, &array[1]);
    es_container_push(&int_container, &array[2]);
    es_container_push(&int_container, &array[2]);
    es_container_push(&int_container, &array[2]);
    es_container_push(&int_container, &array[2]);
    es_container_summary(&int_container);
    for (int i = 0; i != int_container.size; ++i)
    {
        const int value = i * i;
        es_container_set(&int_container, i, &value);
        printf("%d : (get) %d\n", i, *(int *)es_container_get(&int_container, i));
    }
    {
        const es_iterator_t first = es_container_begin(&int_container);
        const es_iterator_t last = es_container_end(&int_container);
        for (es_iterator_t it = first; it != last;
             es_container_incrementit(&int_container, &it))
        {
            printf("%d: (it) %d\n", es_container_diffit(&int_container, first, it), *(int *)it);
        }
    }

    es_vector_t int_references;
    es_vector_init(&int_references);
    es_vector_reserve(&int_references, int_container.size);
    for (int i = 0; i != int_container.size; ++i)
    {
        es_vector_push(&int_references, es_container_get(&int_container, i));
        printf("vector el %d : %d\n", i, *(int *)int_references.data[i]);
    }
    int additional_value = 8;
    es_vector_insert(&int_references, 3, &additional_value);
    for (int i = 0; i != int_references.size; ++i)
    {
        printf("vector el %d : %d\n", i, *(int *)int_references.data[i]);
    }
    bool (*int_comp)(es_cref_t, es_cref_t) = weird_int_order;
    es_qsort(
        es_vector_begin(&int_references),
        es_vector_end(&int_references),
        int_comp);
    printf("sorted vector:\n");
    for (int i = 0; i != int_references.size; ++i)
    {
        printf("vector el %d : %d\n", i, *(int *)int_references.data[i]);
    }
    for (int i = -1; i < 30; i += 5)
    {
        es_ref_t *begin = es_vector_begin(&int_references);
        es_ref_t *end = es_vector_end(&int_references);
        es_ref_t *pos_insert = es_bsearch(begin, end, &i, int_comp);
        es_size_t pos_find = es_bsearch_find(begin, end, &i, int_comp);
        printf("Insert position of %d in vec is %lu\n", i, pos_insert - begin);
        printf("Position of %d in vec is %u\n", i, pos_find);
    }

    es_container_erase(&int_container, 1);
    es_container_summary(&int_container);
    es_container_clear(&int_container);
    es_container_summary(&int_container);

    es_container_t string;
    es_container_init(&string, sizeof(char));
    es_container_summary(&string);
    es_container_reserve(&string, 4);
    es_container_summary(&string);
    char char_array[] = "erf";
    es_container_push(&string, &char_array[0]);
    es_container_push(&string, &char_array[1]);
    es_container_push(&string, &char_array[2]);
    es_container_summary(&string);
    es_container_free(&string);
    es_container_summary(&string);

    es_map_t map = es_map_create(4, false, es_string_hash, es_string_eq);
    const es_size_t n_strings = 7;
    es_string_t values[] = {
        es_string_create("Hello"),
        es_string_create("World"),
        es_string_create("Greg"),
        es_string_create("C is cool"),
        es_string_create("Maps are cooler"),
        es_string_create("Did you say hashing ?"),
        es_string_create("Did you say Hacking ?")};
    for (int i = 0; i != n_strings; ++i)
    {
        es_map_put(&map, &values[i], &values[i]);
    }
    for (int i = 0; i != n_strings; ++i)
    {
        char *val = es_string_get(es_map_get(&map, &values[i]));
        printf("Map[%s] = %s\n", es_string_get(&values[i]), val);
    }

    es_map_init(&map, 10, false, es_int_hash, es_int_eq);
    int int_values[] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 10};
    for (int i = 0; i != sizeof(int_values) / 4; ++i)
    {
        es_map_put(&map, &int_values[i], &int_values[i]);
    }
    for (int i = 0; i != sizeof(int_values) / 4; ++i)
    {
        int val = *(int *)es_map_get(&map, &int_values[i]);
        printf("Map[%d] = %d\n", int_values[i], val);
    }
}
