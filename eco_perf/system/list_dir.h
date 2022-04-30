#ifndef ECO_PERF_LIST_DIR_H_INCLUDED
#define ECO_PERF_LIST_DIR_H_INCLUDED

#include <dirent.h>

#define MAXIMUM_FILE_NAME_SIZE NAME_MAX

typedef struct DirectoryLister
{
    DIR *directory;
    char next[MAXIMUM_FILE_NAME_SIZE];
} directory_lister_t;

void open_directory_lister(
    directory_lister_t *lister,
    char const *root);

char const *get_next_directory(
    directory_lister_t *lister);

void close_directory_lister(
    directory_lister_t *lister);

void list_directories(char const *path);

#endif
