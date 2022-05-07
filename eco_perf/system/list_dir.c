#include "list_dir.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void directory_lister_open(
    directory_lister_t *lister,
    char const *root)
{
    DIR *dir;

    dir = opendir(root);
    if (dir == NULL)
    {
        perror("Directory not found");
        exit(1);
    }
    lister->directory = dir;
}

char const *directory_lister_next(
    directory_lister_t *lister)
{
    struct dirent *entry;
    entry = readdir(lister->directory);
    if (entry)
    {
        sprintf(lister->next, "%s", entry->d_name);
    }
    else
    {
        lister->next[0] = '\0';
        return NULL;
    }
    return lister->next;
}

void directory_lister_close(directory_lister_t *lister)
{
    closedir(lister->directory);
}

void list_directories(char const *path)
{
    directory_lister_t lister;
    directory_lister_open(&lister, path);
    printf("Directories in \'%s\':\n", path);
    char const *next;
    while ((next = directory_lister_next(&lister)))
    {
        puts(next);
    }
    directory_lister_close(&lister);
}
