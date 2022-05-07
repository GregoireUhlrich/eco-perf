#ifndef ECO_PERF_LIST_DIR_H_INCLUDED
#define ECO_PERF_LIST_DIR_H_INCLUDED

#include <dirent.h>

#define MAXIMUM_FILE_NAME_SIZE NAME_MAX

typedef struct DirectoryLister
{
    DIR *directory;
    char next[MAXIMUM_FILE_NAME_SIZE];
} directory_lister_t;

void directory_lister_open(
    directory_lister_t *lister,
    char const *root);

char const *directory_lister_next(
    directory_lister_t *lister);

void directory_lister_close(
    directory_lister_t *lister);

void list_directories(char const *path);

#endif
