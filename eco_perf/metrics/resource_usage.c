#include "resource_usage.h"
#include <stdio.h>
#include <sys/resource.h>

ResourceUsage create_resource_usage()
{
    ResourceUsage usage;
    struct rusage c_usage;
    if (getrusage(RUSAGE_SELF, &c_usage) != 0)
    {
        puts("Error: unavailable resource metrics...");
    }
    else
    {
        usage.user_time = get_time_from_glibc(&c_usage.ru_utime);
        usage.system_time = get_time_from_glibc(&c_usage.ru_stime);
        usage.memory = c_usage.ru_maxrss;
    }
    return usage;
}

void print_resource_usage(ResourceUsage const *usage)
{
    printf("Reource usage:\n");
    printf("  - usr time (s):   %f\n", usage->user_time);
    printf("  - sys time (s):    %f\n", usage->system_time);
    printf("  - max memory (kB):  %ld\n", usage->memory);
}

rtime_t get_time_from_glibc(struct timeval const *c_time)
{
    return c_time->tv_sec + c_time->tv_usec * 1e-6;
}
