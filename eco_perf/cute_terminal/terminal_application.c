#include "terminal_application.h"
#include "definitions/constants.h"
#include <stdio.h>
#include <unistd.h>

void init_terminal_application(
    terminal_application_t *app,
    twidget_t *main_widget)
{
    init_terminal_twidget(&app->terminal, main_widget);
    app->sleep_duration = CT_DEFAULT_SLEEP_DURATION;
}

void update_terminal_application(terminal_application_t *app)
{
    update_twidget(&app->terminal);
    draw_twidget(&app->terminal);
    fflush(stdout);
    usleep(app->sleep_duration * 1e6);
}

void run_terminal_application(terminal_application_t *app)
{
    while (1)
    {
        update_terminal_application(app);
    }
}
