#include "string_utils.h"

char *str_append(char *destination, char const *src)
{
    while (*src)
    {
        *destination++ = *src++;
    }
    *destination = '\0';
    return destination;
}

char *fill_str(char *destination, char filler, int n_repeat)
{
    for (int i = 0; i != n_repeat; ++i)
    {
        destination[i] = filler;
    }
    destination[n_repeat] = '\0';
    return destination;
}
