#ifndef CUTE_TERMINAL_TEXT_LINE_H_INCLUDED
#define CUTE_TERMINAL_TEXT_LINE_H_INCLUDED

#include "twidget.h"

typedef twidget_t text_line_twidget_t;

typedef struct TextLineTWidgetData
{
    char const *_line;
    int _effective_line_length;
} text_line_twidget_data_t;

void init_text_line_twidget(
    text_line_twidget_t *widget,
    text_line_twidget_data_t *data);

void init_text_line_twidget_data(text_line_twidget_data_t *data);

void set_text_line_twidget_data(
    text_line_twidget_t *line_widget,
    char const *line);

#endif
