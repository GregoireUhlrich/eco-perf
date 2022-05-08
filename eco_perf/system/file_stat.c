#include "file_stat.h"
#include <sys/stat.h>
#include <sys/types.h>

time_t get_file_time_last_modified(char const *file_name)
{
    struct stat file_stat;
    stat(file_name, &file_stat);
    return file_stat.st_mtime;
}
