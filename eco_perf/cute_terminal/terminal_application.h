#ifndef CUTE_TERMINAL_APPLICATION_H_INCLUDED
#define CUTE_TERMINAL_APPLICATION_H_INCLUDED

#include "widgets/terminal.h"

typedef struct TerminalApplication
{
    int is_open;
    terminal_tmanager_t terminal_manager;
    float sleep_duration; // in seconds
} terminal_application_t;

void init_terminal_application(
    terminal_application_t *app,
    twidget_t *main_widget);

void update_terminal_application(terminal_application_t *app);

void run_terminal_application(terminal_application_t *app);

void free_terminal_application(terminal_application_t *app);

#endif
