#include "percent_bar.h"
#include "../definitions/error.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _update_percent_bar(percent_bar_twidget_t *percent_bar)
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
    int line_size = get_effective_string_length(buffer);
    printf("%s", buffer);
    move_cursor_left(line_size);
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

void init_percent_bar_twidget_container(
    percent_bar_twidget_container_t *container)
{
    init_percent_bar_config(&container->config);
    init_percent_bar_data(&container->data);
    init_percent_bar_twidget(
        &container->widget,
        &container->data,
        &container->config);
}
