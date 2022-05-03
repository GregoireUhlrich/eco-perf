#include "eco_perf/eco_std/es_string.h"

int main()
{
    es_string_t hello = es_string_create();
    es_string_assign(&hello, "Heeeello ");

    es_string_t *world = es_string_create_new();
    es_string_assign(world, "World!");

    es_string_t hello_world;
    es_string_init(&hello_world);
    es_string_concat(&hello_world, &hello, world);

    es_string_print(&hello);
    es_string_print(world);
    es_string_print(&hello_world);

    es_string_clear(&hello);
    es_string_clear(world);
    es_string_clear(&hello_world);

    es_string_print(&hello);
    es_string_print(world);
    es_string_print(&hello_world);

    es_string_free(&hello);
    es_string_delete(world);
    es_string_free(&hello_world);
}
