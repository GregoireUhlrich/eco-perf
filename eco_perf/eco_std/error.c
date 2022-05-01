#include "error.h"

static const char *error_strings[7];

static void _init_error_strings()
{
    error_strings[ES_TYPE_ERROR] = "eco_std.TypeError";
    error_strings[ES_VALUE_ERROR] = "eco_std.ValueError";
    error_strings[ES_INDEX_ERROR] = "eco_std.IndexError";
    error_strings[ES_OVERFLOW_ERROR] = "eco_std.OverflowError";
    error_strings[ES_MEMORY_ERROR] = "eco_std.MemoryError";
    error_strings[ES_IO_ERROR] = "eco_std.IOError";
    error_strings[ES_RUNTIME_ERROR] = "eco_std.RuntimeError";
}

char const *get_es_error_str(es_error_t error_code)
{
    static int error_strings_initialized = 0;
    if (!error_strings_initialized)
    {
        _init_error_strings();
        error_strings_initialized = 1;
    }
    return error_strings[error_code];
}
