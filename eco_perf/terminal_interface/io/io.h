#ifndef ECO_PERF_IO_H_INCLUDED
#define ECO_PERF_IO_H_INCLUDED

#include "format_definitions.h"

char *str_append(char *destination, char const *src);
char *fill_str(char *destination, char filler, int n_repeat);

char *apply_foreground_color(char *destination, char const *str, term_color_t color);
char *apply_background_color(char *destination, char const *str, term_color_t color);
char *apply_format(char *destination, char const *str, term_format_t format);

void test_colors();
void test_formats();

#endif
