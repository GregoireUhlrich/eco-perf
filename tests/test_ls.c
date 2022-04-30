#include "eco_perf/metrics/process.h"
#include "eco_perf/system/list_dir.h"

int main()
{
    list_directories("./");
    list_directories("/home");

    list_processes();

    return 0;
}
