#include "text_line.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _draw_line_widget(twidget_t *line_widget);

const twidget_interface_t text_line_twidget_interface = {
    default_twidget_update,
    _draw_line_widget,
    default_twidget_free};

void init_text_line_tstack(
    text_line_tstack_t *stack)
{
    twidget_t *widget = &stack->twidget;
    twidget_init(widget);
    text_line_twidget_data_init(&stack->data);
    text_line_twidget_config_init(&stack->config);
    widget->size.y = 1;
    widget->fixed_size.x = 1;
    widget->fixed_size.y = 1;
    widget->stack = (void *)stack;
    widget->interface = &text_line_twidget_interface;
}

void text_line_twidget_data_init(text_line_twidget_data_t *data)
{
    data->_line = "";
    data->_effective_line_length = 0;
}

void text_line_twidget_config_init(text_line_twidget_config_t *config)
{
}

void text_line_set_content(
    text_line_tstack_t *line_stack,
    char const *line)
{
    text_line_twidget_data_t *data = &line_stack->data;
    data->_line = line;
    data->_effective_line_length = get_effective_string_length(line);
    line_stack->twidget.size.x = data->_effective_line_length;
}

void _draw_line_widget(twidget_t *line_widget)
{
    text_line_tstack_t *stack = (text_line_tstack_t *)line_widget;
    text_line_twidget_data_t *data = &stack->data;
    printf("%s", data->_line);
    move_cursor_left(data->_effective_line_length);
}
