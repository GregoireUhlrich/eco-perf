#include "eco_perf/eco_std/container.h"
#include <stdio.h>

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
