#ifndef CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_BOX_H_INCLUDED

#include "twidget.h"

typedef struct BoxTWidgetConfig
{
    char background;
} box_twidget_config_t;

typedef struct BoxTStack
{
    twidget_t twidget;
    box_twidget_config_t config;
} box_tstack_t;

extern const twidget_interface_t box_twidget_interface;

void init_box_tstack(
    box_tstack_t *box);

#endif
