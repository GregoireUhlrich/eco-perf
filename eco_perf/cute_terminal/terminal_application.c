#include "terminal_application.h"
#include "definitions/constants.h"
#include <stdio.h>
#include <unistd.h>

void init_terminal_application(
    terminal_application_t *app,
    twidget_t *main_twidget)
{
    terminal_tstack_init(&app->terminal_stack, main_twidget);
    app->sleep_duration = CT_DEFAULT_SLEEP_DURATION;
    app->is_open = 0;
}

void update_terminal_application(terminal_application_t *app)
{
    twidget_t *terminal_twidget = &app->terminal_stack.terminal_container_twidget;
    update_twidget(terminal_twidget);
    twidget_draw(terminal_twidget);
    fflush(stdout);
    usleep(app->sleep_duration * 1e6);
}

void run_terminal_application(terminal_application_t *app)
{
    app->is_open = 1;
    while (app->is_open)
    {
        update_terminal_application(app);
    }
}

void free_terminal_application(terminal_application_t *app)
{
    twidget_t *terminal_twidget = &app->terminal_stack.terminal_container_twidget;
#ifndef DISABLE_TERMINAL_DRAWING
    terminal_twidget->interface->draw(terminal_twidget);
#endif
    twidget_free(terminal_twidget);
}
