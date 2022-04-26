#ifndef CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED
#define CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED

#include "twidget.h"

typedef twidget_t terminal_twidget_t;

void init_terminal_twidget(
    terminal_twidget_t *terminal,
    twidget_t *main_drawable);

#endif
