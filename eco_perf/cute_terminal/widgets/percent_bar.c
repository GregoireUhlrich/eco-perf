#include "percent_bar.h"
#include "../definitions/error.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _update_percent_bar(twidget_t *percent_bar);
void _draw_percent_bar(twidget_t *percent_bar);

const twidget_interface_t percent_bar_twidget_interface = {
    _update_percent_bar,
    _draw_percent_bar,
    default_twidget_free};

void init_percent_bar_twidget(
    twidget_t *percent_bar,
    percent_bar_data_t *data,
    percent_bar_config_t *config)
{
}

void init_percent_bar_tmanager(
    percent_bar_tmanager_t *manager)
{
    init_percent_bar_config(&manager->config);
    init_percent_bar_data(&manager->data);

    twidget_t *twidget = &manager->twidget;
    init_twidget(twidget);
    twidget->size.y = 1;
    twidget->fixed_size.y = 1;
    twidget->data = (void *)&manager->data;
    twidget->config = (void *)&manager->config;
    twidget->interface = &percent_bar_twidget_interface;
}

void _set_bar_cursor_pos(terminal_vector_t bar_pos)
{
    move_cursor_down(bar_pos.y);
    move_cursor_right(bar_pos.x);
}

void _reset_bar_cursor_pos(
    terminal_vector_t bar_pos,
    int str_size)
{
    move_cursor_up(bar_pos.y);
    move_cursor_left(bar_pos.x + str_size);
}

void _update_percent_bar(twidget_t *percent_bar)
{
    percent_bar_config_t *config = (percent_bar_config_t *)percent_bar->config;
    int percent_size = 6 * (config->percent_mode == CT_PERCENT_OUT);
    config->bar_size = percent_bar->size.x - 2 - percent_size;
    CT_ASSERT(
        config->bar_size > 0,
        CT_VALUE_ERROR,
        "Widget of size %d is too small for percent bar widget.",
        percent_bar->size.x)
}

void _draw_percent_bar(twidget_t *percent_bar)
{
    char buffer[CT_MAX_PERCENT_BAR_SIZE];
    percent_bar_data_t *data = (percent_bar_data_t *)percent_bar->data;
    percent_bar_config_t *config = (percent_bar_config_t *)percent_bar->config;
    create_percent_bar(buffer, data, config);
    int line_size = get_effective_string_length(buffer);
    printf("%s", buffer);
    move_cursor_left(line_size);
}
