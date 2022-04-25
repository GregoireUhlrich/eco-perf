#ifndef ECO_PERF_MEMORY_H_INCLUDED
#define ECO_PERF_MEMORY_H_INCLUDED

void *reallocate_array_memory(
    void *array,
    unsigned int *memory_size,
    unsigned int object_size);

#endif
