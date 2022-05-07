#include "io.h"
#include "error.h"
#include <stdio.h>

es_size_t _read_file(
    FILE *file,
    char *buffer,
    es_size_t buffer_size);

es_size_t es_read_file(
    char const *file_name,
    char *buffer,
    es_size_t buffer_size)
{
    FILE *file = fopen(file_name, "r");
    ES_ASSERT(
        file,
        ES_IO_ERROR,
        "File \'%s\' not found",
        file_name)
    const es_size_t res = _read_file(file, buffer, buffer_size);
    fclose(file);
    return res;
}

es_size_t _read_file(
    FILE *file,
    char *buffer,
    es_size_t buffer_size)
{
    ssize_t res = fread(buffer, buffer_size, 1, file);
    return (es_size_t)res;
}
