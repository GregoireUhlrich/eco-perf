#include "list_dir.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void open_directory_lister(
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

char const *get_next_directory(
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

void close_directory_lister(directory_lister_t *lister)
{
    closedir(lister->directory);
}

void list_directories(char const *path)
{
    directory_lister_t lister;
    open_directory_lister(&lister, path);
    printf("Directories in \'%s\':\n", path);
    char const *next;
    while ((next = get_next_directory(&lister)))
    {
        puts(next);
    }
    close_directory_lister(&lister);
}
