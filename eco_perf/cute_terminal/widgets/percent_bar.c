#include "percent_bar.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _update_percent_bar(percent_bar_twidget_t *percent_bar)
{
    percent_bar_config_t *config = (percent_bar_config_t *)percent_bar->config;
    int percent_size = 6 * (config->percent_mode == PERCENT_OUT);
    config->bar_size = percent_bar->size.x - 2 - percent_size;
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

int _draw_percent_bar(percent_bar_twidget_t const *percent_bar)
{
    char buffer[CT_MAX_PERCENT_BAR_SIZE];
    percent_bar_data_t *data = (percent_bar_data_t *)percent_bar->data;
    percent_bar_config_t *config = (percent_bar_config_t *)percent_bar->config;
    create_percent_bar(buffer, data, config);
    save_cursor_position();
    printf("%s", buffer);
    restore_cursor_position();
    return 1;
}

void init_percent_bar_twidget(
    percent_bar_twidget_t *percent_bar,
    percent_bar_data_t *data,
    percent_bar_config_t *config)
{
    init_twidget(percent_bar);
    percent_bar->size.y = 1;
    percent_bar->data = (void *)data;
    percent_bar->config = (void *)config;
    percent_bar->update = _update_percent_bar;
    percent_bar->draw_self = _draw_percent_bar;
}
