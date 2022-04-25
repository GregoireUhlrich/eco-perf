#ifndef ECO_PERF_TERMINAL_PANEL_H_INCLUDED
#define ECO_PERF_TERMINAL_PANEL_H_INCLUDED

#include "term_drawable.h"

typedef term_drawable_t terminal_panel_t;

void init_terminal_panel(
    terminal_panel_t *terminal,
    term_drawable_t *main_drawable);

#endif
