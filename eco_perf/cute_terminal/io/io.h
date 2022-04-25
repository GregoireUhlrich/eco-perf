#ifndef ECO_PERF_IO_H_INCLUDED
#define ECO_PERF_IO_H_INCLUDED

#include "format_definitions.h"

char *apply_foreground_color(char *destination, char const *str, term_color_t color);
char *apply_background_color(char *destination, char const *str, term_color_t color);
char *apply_format(char *destination, char const *str, term_format_t format);

void test_colors();
void test_formats();

#endif
