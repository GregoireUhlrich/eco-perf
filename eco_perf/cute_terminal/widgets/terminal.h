#ifndef CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED
#define CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED

#include "twidget.h"

typedef struct TerminalTStack
{
    twidget_t terminal_container_twidget;
    twidget_t *main_twidget;
} terminal_tstack_t;

extern const twidget_interface_t terminal_twidget_interface;

void terminal_tstack_init(
    terminal_tstack_t *terminal,
    twidget_t *main_twidget);

#endif
