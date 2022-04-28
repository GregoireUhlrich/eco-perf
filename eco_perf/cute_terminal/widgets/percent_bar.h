#ifndef CUTE_TERMINAL_PERCENT_BAR_TWIDGET_H_INCLUDED
#define CUTE_TERMINAL_PERCENT_BAR_TWIDGET_H_INCLUDED

#include "../tools/percent_bar.h"
#include "twidget.h"

typedef struct PercentBarTManager
{
    twidget_t twidget;
    percent_bar_config_t config;
    percent_bar_data_t data;
} percent_bar_tmanager_t;

extern const twidget_interface_t percent_bar_twidget_interface;

void init_percent_bar_tmanager(
    percent_bar_tmanager_t *container);

#endif
