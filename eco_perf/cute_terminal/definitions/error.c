#include "error.h"

static const char *error_strings[6];

void _init_error_strings()
{
    error_strings[CT_TYPE_ERROR] = "CuteTerminal.TypeError";
    error_strings[CT_VALUE_ERROR] = "CuteTerminal.ValueError";
    error_strings[CT_INDEX_ERROR] = "CuteTerminal.IndexError";
    error_strings[CT_OVERFLOW_ERROR] = "CuteTerminal.OverflowError";
    error_strings[CT_IO_ERROR] = "CuteTerminal.IOError";
    error_strings[CT_RUNTIME_ERROR] = "CuteTerminal.RuntimeError";
}

char const *get_ct_error_str(ct_error_t error_code)
{
    static int error_strings_initialized = 0;
    if (!error_strings_initialized)
    {
        _init_error_strings();
        error_strings_initialized = 1;
    }
    return error_strings[error_code];
}
