#include "eco_perf/eco_std/container.h"

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
