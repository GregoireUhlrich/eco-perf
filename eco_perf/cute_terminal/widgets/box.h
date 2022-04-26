#ifndef CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED

#include "twidget.h"

typedef twidget_t term_box_t;

typedef struct BoxTWidgetConfig
{
    char background;
} box_twidget_t;

void init_box_twidget(
    term_box_t *box,
    terminal_vector_t pos,
    terminal_vector_t size);

void set_box_twidget_config(
    term_box_t *box,
    box_twidget_t *config);

#endif
