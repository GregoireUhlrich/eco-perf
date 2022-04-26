#include "text_line.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _update_line_widget(text_line_twidget_t *line_widget);
int _draw_line_widget(text_line_twidget_t const *line_widget);

void init_text_line_twidget(
    text_line_twidget_t *widget,
    text_line_twidget_data_t *data)
{
    init_twidget(widget);
    widget->data = (void *)data;
    widget->update = _update_line_widget;
    widget->draw_self = _draw_line_widget;
}

void init_text_line_twidget_data(text_line_twidget_data_t *data)
{
    data->_line = "";
    data->_effective_line_length = 0;
}

void set_text_line_twidget_data(
    text_line_twidget_t *line_widget,
    char const *line)
{
    text_line_twidget_data_t *data = (text_line_twidget_data_t *)line_widget->data;
    data->_line = line;
    data->_effective_line_length = get_effective_string_length(line);
    line_widget->size.x = data->_effective_line_length;
}

void _update_line_widget(text_line_twidget_t *line_widget)
{
}

int _draw_line_widget(text_line_twidget_t const *line_widget)
{
    text_line_twidget_data_t *data = (text_line_twidget_data_t *)line_widget->data;
    printf("%s", data->_line);
    move_cursor_left(data->_effective_line_length);
    return 1;
}
