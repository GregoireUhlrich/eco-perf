#include "io.h"
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

char *_apply_format(char *destination, char const *str, char const *init_format, char const *final_format)
{
    destination = str_append(destination, init_format);
    destination = str_append(destination, str);
    destination = str_append(destination, final_format);
    return destination;
}

char *apply_foreground_color(char *destination, char const *str, term_color_t color)
{
    char const *init_color = get_foreground_color(color);
    char const *final_color = get_foreground_color(DEFAULT_COLOR);
    return _apply_format(destination, str, init_color, final_color);
}

char *apply_background_color(char *destination, char const *str, term_color_t color)
{
    char const *init_color = get_background_color(color);
    char const *final_color = get_background_color(DEFAULT_COLOR);
    return _apply_format(destination, str, init_color, final_color);
}

char *apply_format(char *destination, char const *str, term_format_t format)
{
    char const *init_format = get_format(format);
    char const *final_format = get_format(DEFAULT_FORMAT);
    return _apply_format(destination, str, init_format, final_format);
}

void _test_color(char const *name, term_color_t color)
{
    char buffer[100];
    char foreground_str[100];
    char background_str[100];

    strcpy(foreground_str, name);
    strcpy(background_str, name);
    apply_foreground_color(buffer, foreground_str, color);
    printf("%s ", buffer);
    apply_background_color(buffer, background_str, color);
    printf("%s\n", buffer);
}

void test_colors()
{
    _test_color("DEFAULT_COLOR", DEFAULT_COLOR);
    _test_color("BLACK", BLACK);
    _test_color("RED", RED);
    _test_color("GREEN", GREEN);
    _test_color("YELLOW", YELLOW);
    _test_color("BLUE", BLUE);
    _test_color("MAGENTA", MAGENTA);
    _test_color("CYAN", CYAN);
    _test_color("LIGHT_GRAY", LIGHT_GRAY);
    _test_color("DARK_GRAY", DARK_GRAY);
    _test_color("LIGHT_RED", LIGHT_RED);
    _test_color("LIGHT_GREEN", LIGHT_GREEN);
    _test_color("LIGHT_YELLOW", LIGHT_YELLOW);
    _test_color("LIGHT_BLUE", LIGHT_BLUE);
    _test_color("LIGHT_MAGENTA", LIGHT_MAGENTA);
    _test_color("LIGHT_CYAN", LIGHT_CYAN);
    _test_color("WHITE", WHITE);
}

void _test_format(char const *name, term_format_t format)
{
    char buffer[100];
    apply_format(buffer, name, format);
    printf("%s: %s\n", name, buffer);
}

void test_formats()
{
    _test_format("DEFAULT_FORMAT", DEFAULT_FORMAT);
    _test_format("BOLD", BOLD);
    _test_format("DIM", DIM);
    _test_format("UNDERLINED", UNDERLINED);
    _test_format("BLINKING", BLINKING);
    _test_format("INVERTED", INVERTED);
    _test_format("HIDDEN", HIDDEN);
}
