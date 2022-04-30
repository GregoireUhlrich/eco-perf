#include "eco_perf/eco_std/vector.h"

int main()
{
    es_vector_t int_vector;
    es_vector_init(&int_vector, sizeof(int));
    es_vector_summary(&int_vector);
    es_vector_reserve(&int_vector, 4);
    es_vector_summary(&int_vector);
    int array[] = {1, 2, 3};
    es_vector_push(&int_vector, &array[0]);
    es_vector_push(&int_vector, &array[1]);
    es_vector_push(&int_vector, &array[2]);
    es_vector_summary(&int_vector);
    es_vector_erase(&int_vector, 1);
    es_vector_summary(&int_vector);
    es_vector_clear(&int_vector);
    es_vector_summary(&int_vector);

    es_vector_t string;
    es_vector_init(&string, sizeof(char));
    es_vector_summary(&string);
    es_vector_reserve(&string, 4);
    es_vector_summary(&string);
    char char_array[] = "erf";
    es_vector_push(&string, &char_array[0]);
    es_vector_push(&string, &char_array[1]);
    es_vector_push(&string, &char_array[2]);
    es_vector_summary(&string);
    es_vector_free(&string);
    es_vector_summary(&string);
}
