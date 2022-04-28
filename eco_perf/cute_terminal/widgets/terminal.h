#ifndef CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED
#define CUTE_TERMINAL_TERMINAL_TWIDGET_H_INCLUDED

#include "twidget.h"

typedef struct TerminalTManager
{
    twidget_t terminal_container_twidget;
    twidget_t *main_twidget;
} terminal_tmanager_t;

extern const twidget_interface_t terminal_twidget_interface;

void init_terminal_tmanager(
    terminal_tmanager_t *terminal,
    twidget_t *main_twidget);

#endif
