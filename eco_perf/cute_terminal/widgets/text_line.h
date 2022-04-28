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

typedef struct TextLineTManager
{
    twidget_t twidget;
    text_line_twidget_data_t data;
    text_line_twidget_config_t config;
} text_line_tmanager_t;

extern const twidget_interface_t text_line_twidget_interface;

void init_text_line_tmanager(
    text_line_tmanager_t *manager);

void init_text_line_twidget_data(text_line_twidget_data_t *data);
void init_text_line_twidget_config(text_line_twidget_config_t *config);

void set_text_line_content(
    text_line_tmanager_t *line_manager,
    char const *line);

#endif
