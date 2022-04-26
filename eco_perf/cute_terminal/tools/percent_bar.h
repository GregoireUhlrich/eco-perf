#ifndef CUTE_TERMINAL_PERCENT_BAR_H_INCLUDED
#define CUTE_TERMINAL_PERCENT_BAR_H_INCLUDED

#include "../definitions/limits.h"
#include "../io/format_definitions.h"
#include "../io/io.h"

char *str_append(char *destination, char const *src);

typedef enum DisplayPercentMode
{
    CT_NO_PERCENT,
    CT_PERCENT_IN,
    CT_PERCENT_OUT,
} display_percent_mode_t;

typedef struct PercentBarConfig
{
    int bar_size;
    display_percent_mode_t percent_mode;

    char *left;
    char *right;
    char fill;
    char empty;
} percent_bar_config_t;

void init_percent_bar_config(percent_bar_config_t *config);

typedef struct PercentBarData
{
    int n_data;
    double data[CT_MAX_PERCENT_BAR_N_DATA];
    term_color_t colors[CT_MAX_PERCENT_BAR_N_DATA];
} percent_bar_data_t;

void init_percent_bar_data(percent_bar_data_t *data);

void add_percent_data(
    percent_bar_data_t *data,
    double ratio,
    term_color_t color);

int create_percent_bar(
    char *buffer,
    percent_bar_data_t const *data,
    percent_bar_config_t const *config);

#endif
