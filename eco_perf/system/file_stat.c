#include "file_stat.h"
#include <sys/stat.h>
#include <sys/types.h>

time_t get_file_time_last_modified(char const *file_name)
{
    struct stat file_stat;
    int res = stat(file_name, &file_stat);
    if (res != 0)
    {
        return -1;
    }
    return file_stat.st_atim.tv_sec;
}
