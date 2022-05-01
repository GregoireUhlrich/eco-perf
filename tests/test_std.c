#include "eco_perf/eco_std/algo.h"
#include "eco_perf/eco_std/container.h"
#include "eco_perf/eco_std/vector.h"
#include <stdio.h>

ES_DEFINE_DEFAULT_COMP(int, int_comparator)

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
    for (int i = -1; i < 30; i += 5)
    {
        es_ref_t *begin = es_vector_begin(&int_references);
        es_ref_t *end = es_vector_end(&int_references);
        es_ref_t *pos_insert = es_bsearch(begin, end, &i, int_comparator);
        es_size_t pos_find = es_bsearch_find(begin, end, &i, int_comparator);
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
}
