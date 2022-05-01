#ifndef ECO_STD_ERROR_H_INCLUDED
#define ECO_STD_ERROR_H_INCLUDED

#define ES_ERROR_STR_BUFFER_SIZE 1024

typedef enum ESError
{

    ES_TYPE_ERROR,
    ES_VALUE_ERROR,
    ES_INDEX_ERROR,
    ES_OVERFLOW_ERROR,
    ES_MEMORY_ERROR,
    ES_IO_ERROR,
    ES_RUNTIME_ERROR
} es_error_t;

const char *get_es_error_str(es_error_t error_code);

#ifdef ES_ECO_MODE
#define ES_ERROR(error_code, message, ...)
#define ES_ASSERT(condition, error_code, message, ...)
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ES_ERROR(error_code, message, ...)                   \
    {                                                        \
        char _es_error_str_buffer[ES_ERROR_STR_BUFFER_SIZE]; \
        sprintf(                                             \
            _es_error_str_buffer,                            \
            "In function %s (l.%d %s)\n%s : " message "\n",  \
            __func__,                                        \
            __LINE__,                                        \
            __FILE__,                                        \
            get_es_error_str(error_code),                    \
            ##__VA_ARGS__);                                  \
        fprintf(stderr, "%s", _es_error_str_buffer);         \
        exit(1);                                             \
    }

#define ES_ASSERT(condition, error_code, message, ...)   \
    {                                                    \
        if (!(condition))                                \
        {                                                \
            ES_ERROR(error_code, message, ##__VA_ARGS__) \
        }                                                \
    }
#endif

#endif
