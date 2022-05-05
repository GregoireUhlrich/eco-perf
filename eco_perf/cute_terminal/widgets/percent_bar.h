#ifndef CUTE_TERMINAL_PERCENT_BAR_TWIDGET_H_INCLUDED
#define CUTE_TERMINAL_PERCENT_BAR_TWIDGET_H_INCLUDED

#include "../tools/percent_bar.h"
#include "twidget.h"

typedef struct PercentBarTStack
{
    twidget_t twidget;
    percent_bar_config_t config;
    percent_bar_data_t data;
} percent_bar_tstack_t;

extern const twidget_interface_t percent_bar_twidget_interface;

void percent_bar_tstack_init(
    percent_bar_tstack_t *container);

percent_bar_tstack_t percent_bar_data_create();

#endif
