#ifndef ECO_PERF_FORMAT_DEFINITION_H_INCLUDED
#define ECO_PERF_FORMAT_DEFINITION_H_INCLUDED

typedef enum TermColor
{
    CT_NULL_COLOR = -1,
    CT_DEFAULT_COLOR = 0,
    CT_BLACK,
    CT_RED,
    CT_GREEN,
    CT_YELLOW,
    CT_BLUE,
    CT_MAGENTA,
    CT_CYAN,
    CT_LIGHT_GRAY,
    CT_DARK_GRAY,
    CT_LIGHT_RED,
    CT_LIGHT_GREEN,
    CT_LIGHT_YELLOW,
    CT_LIGHT_BLUE,
    CT_LIGHT_MAGENTA,
    CT_LIGHT_CYAN,
    CT_WHITE,
} term_color_t;

typedef struct ColorDefinitions
{
    char *default_color;
    char *black;
    char *red;
    char *green;
    char *yellow;
    char *blue;
    char *magenta;
    char *cyan;
    char *light_gray;
    char *dark_gray;
    char *light_red;
    char *light_green;
    char *light_yellow;
    char *light_blue;
    char *light_magenta;
    char *light_cyan;
    char *white;
} color_definitions_t;

char const *get_foreground_color(term_color_t color);
char const *get_background_color(term_color_t color);

typedef enum TermFormat
{
    NULL_FORMAT = -1,
    DEFAULT_FORMAT = 0,
    BOLD,
    DIM,
    UNDERLINED,
    BLINKING,
    INVERTED,
    HIDDEN,
} term_format_t;

typedef struct FormatDefinitions
{
    char *default_format;
    char *bold;
    char *dim;
    char *underlined;
    char *blinking;
    char *inverted;
    char *hidden;
} format_definitions_t;

char const *get_format(term_format_t format);

#endif
