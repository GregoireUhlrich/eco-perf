#include "io.h"
#include "string_utils.h"
#include <stdio.h>
#include <string.h>

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
    char const *final_color = get_foreground_color(CT_DEFAULT_COLOR);
    return _apply_format(destination, str, init_color, final_color);
}

char *apply_background_color(char *destination, char const *str, term_color_t color)
{
    char const *init_color = get_background_color(color);
    char const *final_color = get_background_color(CT_DEFAULT_COLOR);
    return _apply_format(destination, str, init_color, final_color);
}

char *apply_format(char *destination, char const *str, term_format_t format)
{
    char const *init_format = get_format(format);
    char const *final_format = get_format(CT_DEFAULT_FORMAT);
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
    _test_color("DEFAULT_COLOR", CT_DEFAULT_COLOR);
    _test_color("BLACK", CT_BLACK);
    _test_color("RED", CT_RED);
    _test_color("GREEN", CT_GREEN);
    _test_color("YELLOW", CT_YELLOW);
    _test_color("BLUE", CT_BLUE);
    _test_color("MAGENTA", CT_MAGENTA);
    _test_color("CYAN", CT_CYAN);
    _test_color("LIGHT_GRAY", CT_LIGHT_GRAY);
    _test_color("DARK_GRAY", CT_DARK_GRAY);
    _test_color("LIGHT_RED", CT_LIGHT_RED);
    _test_color("LIGHT_GREEN", CT_LIGHT_GREEN);
    _test_color("LIGHT_YELLOW", CT_LIGHT_YELLOW);
    _test_color("LIGHT_BLUE", CT_LIGHT_BLUE);
    _test_color("LIGHT_MAGENTA", CT_LIGHT_MAGENTA);
    _test_color("LIGHT_CYAN", CT_LIGHT_CYAN);
    _test_color("WHITE", CT_WHITE);
}

void _test_format(char const *name, term_format_t format)
{
    char buffer[100];
    apply_format(buffer, name, format);
    printf("%s: %s\n", name, buffer);
}

void test_formats()
{
    _test_format("DEFAULT_FORMAT", CT_DEFAULT_FORMAT);
    _test_format("BOLD", CT_BOLD);
    _test_format("DIM", CT_DIM);
    _test_format("UNDERLINED", CT_UNDERLINED);
    _test_format("BLINKING", CT_BLINKING);
    _test_format("INVERTED", CT_INVERTED);
    _test_format("HIDDEN", CT_HIDDEN);
}
