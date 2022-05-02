#ifndef ECO_STD_IO_H_INCLUDED
#define ECO_STD_IO_H_INCLUDED

#include "def.h"

es_size_t es_read_file(
    char const *file_name,
    char *buffer,
    es_size_t buffer_size);

#endif
