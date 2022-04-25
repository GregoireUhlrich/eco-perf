#include "percent_bar.h"
#include "../io/io.h"
#include "../io/string_utils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

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

int create_percent_bar(
    char *destination,
    percent_bar_data_t const *data,
    percent_bar_config_t const *config)
{
    char buffer[MAX_PERCENT_BAR_SIZE];
    destination = apply_format(destination, config->left, BOLD);
    double value_tot = 0;
    int n_tot = 0;
    for (int i = 0; i != data->n_data; ++i)
    {
        const double value = data->data[i] * config->bar_size;
        int n_chars = ceil(value);
        value_tot += value;
        n_tot += n_chars;
        if (n_tot >= config->bar_size)
        {
            n_chars -= (n_tot - config->bar_size);
            n_tot = config->bar_size;
        }
        if (n_chars == 0)
            continue;
        fill_str(buffer, config->fill, n_chars);
        destination = apply_foreground_color(destination, buffer, data->colors[i]);
    }
    int n_blank = config->bar_size - n_tot;
    if (n_blank < 0)
        n_blank = 0;
    fill_str(buffer, config->empty, n_blank);
    destination = str_append(destination, buffer);
    destination = apply_format(destination, config->right, BOLD);
    int str_size = 2 + config->bar_size;
    if (config->percent_mode == PERCENT_OUT)
    {
        double final_percentage = value_tot * 100. / config->bar_size;
        if (final_percentage > 100)
            final_percentage = 100;
        sprintf(buffer, " %.1f%% ", final_percentage);
        str_size += strlen(buffer);
        destination = str_append(destination, buffer);
    }
    return str_size;
}
