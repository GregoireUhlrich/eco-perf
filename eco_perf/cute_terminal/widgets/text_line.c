#include "text_line.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _draw_line_widget(twidget_t *line_widget);

const twidget_interface_t text_line_twidget_interface = {
    default_twidget_update,
    _draw_line_widget,
    default_twidget_free};

void init_text_line_tmanager(
    text_line_tmanager_t *manager)
{
    twidget_t *widget = &manager->twidget;
    init_twidget(widget);
    init_text_line_twidget_data(&manager->data);
    init_text_line_twidget_config(&manager->config);
    widget->size.y = 1;
    widget->fixed_size.x = 1;
    widget->fixed_size.y = 1;
    widget->manager = (void *)manager;
    widget->interface = &text_line_twidget_interface;
}

void init_text_line_twidget_data(text_line_twidget_data_t *data)
{
    data->_line = "";
    data->_effective_line_length = 0;
}

void init_text_line_twidget_config(text_line_twidget_config_t *config)
{
}

void set_text_line_content(
    text_line_tmanager_t *line_manager,
    char const *line)
{
    text_line_twidget_data_t *data = &line_manager->data;
    data->_line = line;
    data->_effective_line_length = get_effective_string_length(line);
    line_manager->twidget.size.x = data->_effective_line_length;
}

void _draw_line_widget(twidget_t *line_widget)
{
    text_line_tmanager_t *manager = (text_line_tmanager_t *)line_widget;
    text_line_twidget_data_t *data = &manager->data;
    printf("%s", data->_line);
    move_cursor_left(data->_effective_line_length);
}
