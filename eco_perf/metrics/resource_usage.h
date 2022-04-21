#ifndef ECO_PERF_RESOURCE_USAGE_H_INCLUDED
#define ECO_PERF_RESOURCE_USAGE_H_INCLUDED

// Forward declaration of struct in sys/resource.h
struct timeval;

// Typedefs for metrics
typedef double rtime_t;
typedef long int rmem_t;

typedef struct
{
    rtime_t user_time;
    rtime_t system_time;
    rmem_t memory;
} ResourceUsage;

ResourceUsage create_resource_usage();

void print_resource_usage(ResourceUsage const *usage);

rtime_t get_time_from_glibc(struct timeval const *c_time);

#endif
