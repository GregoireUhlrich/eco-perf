#include "format_definitions.h"

char const *get_foreground_color(term_color_t color)
{
    static color_definitions_t _foreground_color;
    static int colors_defined = 0;
    if (!colors_defined)
    {
        _foreground_color.default_color = "\e[39m";
        _foreground_color.black = "\e[30m";
        _foreground_color.red = "\e[31m";
        _foreground_color.green = "\e[32m";
        _foreground_color.yellow = "\e[33m";
        _foreground_color.blue = "\e[34m";
        _foreground_color.magenta = "\e[35m";
        _foreground_color.cyan = "\e[36m";
        _foreground_color.light_gray = "\e[37m";
        _foreground_color.dark_gray = "\e[90m";
        _foreground_color.light_red = "\e[91m";
        _foreground_color.light_green = "\e[92m";
        _foreground_color.light_yellow = "\e[93m";
        _foreground_color.light_blue = "\e[94m";
        _foreground_color.light_magenta = "\e[95m";
        _foreground_color.light_cyan = "\e[96m";
        _foreground_color.white = "\e[97m";
    }
    return *((&_foreground_color.default_color) + color);
}

char const *get_background_color(term_color_t color)
{
    static color_definitions_t _background_color;
    static int colors_defined = 0;
    if (!colors_defined)
    {
        _background_color.default_color = "\e[49m";
        _background_color.black = "\e[40m";
        _background_color.red = "\e[41m";
        _background_color.green = "\e[42m";
        _background_color.yellow = "\e[43m";
        _background_color.blue = "\e[44m";
        _background_color.magenta = "\e[45m";
        _background_color.cyan = "\e[46m";
        _background_color.light_gray = "\e[47m";
        _background_color.dark_gray = "\e[100m";
        _background_color.light_red = "\e[101m";
        _background_color.light_green = "\e[102m";
        _background_color.light_yellow = "\e[103m";
        _background_color.light_blue = "\e[104m";
        _background_color.light_magenta = "\e[105m";
        _background_color.light_cyan = "\e[106m";
        _background_color.white = "\e[107m";
    }
    return *((&_background_color.default_color) + color);
}

char const *get_format(term_format_t format)
{
    static format_definitions_t _formats;
    static int formats_defined = 0;
    if (!formats_defined)
    {
        _formats.default_format = "\e[0m";
        _formats.bold = "\e[1m";
        _formats.dim = "\e[2m";
        _formats.underlined = "\e[4m";
        _formats.blinking = "\e[5m";
        _formats.inverted = "\e[7m";
        _formats.hidden = "\e[8m";
    }
    return *((&_formats.default_format) + format);
}
