#include "string_utils.h"
#include <stdio.h>
#include <string.h>

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

int is_single_line_string(char const *str)
{
    char c;
    while ((c = *str))
    {
        if (c == '\n' || c == '\t' || c == '\r')
        {
            return 0;
        }
        ++str;
    }
    return 1;
}

int get_effective_string_length(char const *str)
{
    int length = 0;
    char c;
    while ((c = *str))
    {
        if (c == '\e') // Formatting character
        {
            while (c && c != 'm') // Do not count until 'm'
                c = *++str;
            if (!c)
            {
                break;
            }
        }
        else
        {
            ++length;
        }
        // Characteres between 0 and 31 (included)
        // are special characters that are not displayed
        // (length 0) so do not count in the effective length.
        ++str;
    }
    return length;
}

void lfill(
    char *destination,
    char const *str,
    int length,
    char filler)
{
    int n_filled_char = length - strlen(str);
    for (int i = 0; i < n_filled_char; ++i)
        *destination++ = filler;
    sprintf(destination, "%s", str);
}

void rfill(
    char *destination,
    char const *str,
    int length,
    char filler)
{
    const int len = strlen(str);
    int n_filled_char = length - len;
    sprintf(destination, "%s", str);
    destination += len;
    for (int i = 0; i < n_filled_char; ++i)
        *destination++ = filler;
    *destination = '\0';
}

void cfill(
    char *destination,
    char const *str,
    int length,
    char filler)
{
    const int len = strlen(str);
    int n_filled_char = length - len;
    for (int i = 0; i < n_filled_char / 2; ++i)
        *destination++ = filler;
    sprintf(destination, "%s", str);
    destination += len;
    for (int i = 0; i < n_filled_char - n_filled_char / 2; ++i)
        *destination++ = filler;
    *destination = '\0';
}
