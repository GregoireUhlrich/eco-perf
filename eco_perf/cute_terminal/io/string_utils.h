#ifndef CUTE_TERMINAL_STRING_UTILS_H_INCLUDED
#define CUTE_TERMINAL_STRING_UTILS_H_INCLUDED

char *str_append(char *destination, char const *src);
char *fill_str(char *destination, char filler, int n_repeat);
int is_single_line_string(char const *str);
int get_effective_string_length(char const *str);

#endif
