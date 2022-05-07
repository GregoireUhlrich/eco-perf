#include "memory_usage.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static const char scales[] = "kMGTPE";
static const int n_scales = sizeof(scales);

void print_nice_memory(char *destination, unsigned long mem_kb)
{
    int n_scales = (int)(log10(mem_kb) / log10(1024));
    if (n_scales > 0)
    {
        double scaled_mem = mem_kb / pow(1024, n_scales);
        sprintf(destination, "%.2f %cB", scaled_mem, scales[n_scales]);
    }
    else
    {
        sprintf(destination, "%lu kB", mem_kb);
    }
}
