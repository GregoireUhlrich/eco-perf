#ifndef ECO_PERF_MEMORY_USAGE_H_INCLUDED
#define ECO_PERF_MEMORY_USAGE_H_INCLUDED

typedef struct MemoryData
{
    long unsigned int real;   // kB
    long unsigned int virt;   // kB
    long unsigned int shared; // kB
} memory_data_t;

void print_nice_memory(char *destination, unsigned long mem_kb);

#endif
