#ifndef CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED

#include "twidget.h"

typedef twidget_t box_twidget_t;

typedef struct BoxTWidgetConfig
{
    char background;
} box_twidget_config_t;

void init_box_twidget(
    box_twidget_t *box);

void set_box_twidget_config(
    box_twidget_t *box,
    box_twidget_config_t *config);

#endif
