#ifndef CUTE_TERMINAL_APPLICATION_H_INCLUDED
#define CUTE_TERMINAL_APPLICATION_H_INCLUDED

#include "widgets/terminal.h"

typedef struct TerminalApplication
{
    terminal_twidget_t terminal;
    float sleep_duration; // in seconds
} terminal_application_t;

void init_terminal_application(
    terminal_application_t *app,
    twidget_t *main_widget);

void update_terminal_application(terminal_application_t *app);

void run_terminal_application(terminal_application_t *app);

#endif
