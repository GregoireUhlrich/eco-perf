#include "eco_perf/system/list_dir.h"

int main()
{
    list_directories("./");
    list_directories("/proc");
    return 0;
}
