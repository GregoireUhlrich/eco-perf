#include "memory.h"
#include <stdlib.h>
#include <string.h>

void *reallocate_array_memory(
    void *array,
    unsigned int *memory_size,
    unsigned int object_size)
{
    if (*memory_size == 0)
    {
        array = malloc(object_size);
        *memory_size = 1;
        return array;
    }
    void *new_array = malloc(2 * (*memory_size) * object_size);
    memcpy(
        new_array,
        array,
        (*memory_size) * object_size);
    free(array);
    *memory_size *= 2;
    return new_array;
}
