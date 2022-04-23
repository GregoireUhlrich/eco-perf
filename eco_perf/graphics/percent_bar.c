#include "percent_bar.h"
#include <math.h>
#include <stdio.h>

void load_default_bar_config(percent_bar_config_t *config)
{
    config->bar_size = 32;
    config->percent_mode = PERCENT_OUT;
    config->left = "[";
    config->right = "]";
    config->fill = '|';
    config->empty = ' ';
}

void init_percent_bar_data(percent_bar_data_t *data)
{
    data->n_data = 0;
}

void add_percent_data(
    percent_bar_data_t *data,
    double ratio,
    term_color_t color)
{
    data->data[data->n_data] = ratio;
    data->colors[data->n_data] = color;
    ++data->n_data;
}

char *_fill_str(char *destination, char filler, int n_repeat)
{
    for (int i = 0; i != n_repeat; ++i)
    {
        destination[i] = filler;
    }
    destination[n_repeat] = '\0';
    return destination;
}

char *create_percent_bar(
    char *destination,
    percent_bar_data_t const *data,
    percent_bar_config_t const *config)
{
    char buffer[MAX_PERCENT_BAR_SIZE];
    destination = apply_format(destination, config->left, BOLD);
    int n_tot = 0;
    for (int i = 0; i != data->n_data; ++i)
    {
        const int n_chars = round(data->data[i] * config->bar_size);
        if (n_chars == 0)
            continue;
        _fill_str(buffer, config->fill, n_chars);
        destination = apply_foreground_color(destination, buffer, data->colors[i]);
        n_tot += n_chars;
    }
    int n_blank = config->bar_size - n_tot;
    if (n_blank < 0)
        n_blank = 0;
    _fill_str(buffer, config->empty, n_blank);
    destination = str_append(destination, buffer);
    destination = apply_format(destination, config->right, BOLD);
    if (config->percent_mode == PERCENT_OUT)
    {
        sprintf(buffer, " %.1f%%", n_tot * 100. / config->bar_size);
        destination = str_append(destination, buffer);
    }
    return destination;
}
