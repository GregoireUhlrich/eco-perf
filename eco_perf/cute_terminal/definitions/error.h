#ifndef ECO_PERF_ERROR_H_INCLUDED
#define ECO_PERF_ERROR_H_INCLUDED

#include "limits.h"

typedef enum CTError
{

    CT_TYPE_ERROR,
    CT_VALUE_ERROR,
    CT_INDEX_ERROR,
    CT_OVERFLOW_ERROR,
    CT_IO_ERROR,
    CT_RUNTIME_ERROR
} ct_error_t;

const char *get_ct_error_str(ct_error_t error_code);

#ifdef CT_ECO_MODE
#define CT_ERROR(error_code, message, ...)
#else
#include <stdio.h>
#include <string.h>
#define CT_ERROR(error_code, message, ...)                   \
    {                                                        \
        char _ct_error_str_buffer[CT_ERROR_STR_BUFFER_SIZE]; \
        sprintf(                                             \
            _ct_error_str_buffer,                            \
            "%s : " message "\n",                            \
            get_ct_error_str(error_code),                    \
            ##__VA_ARGS__);                                  \
        fprintf(stderr, "%s", _ct_error_str_buffer);         \
        exit(1);                                             \
    }
#endif

#endif
