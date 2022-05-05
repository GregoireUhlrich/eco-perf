#ifndef CUTE_TERMINAL_TEXT_LINE_H_INCLUDED
#define CUTE_TERMINAL_TEXT_LINE_H_INCLUDED

#include "twidget.h"

typedef struct TextLineTWidgetData
{
    char const *_line;
    int _effective_line_length;
} text_line_twidget_data_t;

typedef struct TextLineTWidgetConfig
{

} text_line_twidget_config_t;

typedef struct TextLineTStack
{
    twidget_t twidget;
    text_line_twidget_data_t data;
    text_line_twidget_config_t config;
} text_line_tstack_t;

extern const twidget_interface_t text_line_twidget_interface;

void text_line_tstack_init(
    text_line_tstack_t *stack);

text_line_tstack_t text_line_tstack_create();

void text_line_twidget_data_init(text_line_twidget_data_t *data);
void text_line_twidget_config_init(text_line_twidget_config_t *config);

void text_line_set_content(
    text_line_tstack_t *line_stack,
    char const *line);

#endif
