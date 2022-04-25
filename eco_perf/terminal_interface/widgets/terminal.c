#include "terminal.h"
#include "../terminal/cursor.h"
#include "../terminal/window.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void _update_terminal(terminal_twidget_t *terminal)
{
    terminal_window_t terminal_window;
    init_terminal_window(&terminal_window);
    terminal->size.x = terminal_window.width;
    terminal->size.y = terminal_window.height - 1;
    terminal->pos.x = 0;
    terminal->pos.y = 1;
    if (terminal->children.size == 1)
    {
        terminal->children.widgets[0]->size = terminal->size;
    }
    else
    {
        errno = EINVAL;
        perror("Terminal should have one and one widget child only.");
        exit(1);
    }
}

int _draw_terminal(terminal_twidget_t const *terminal)
{
    clear_terminal();
    return 1;
}

void init_terminal_twidget(
    terminal_twidget_t *terminal,
    twidget_t *main_drawable)
{
    init_twidget(terminal);
    add_twidget_child(terminal, main_drawable);
    terminal->update = _update_terminal;
    terminal->update(terminal);
    terminal->draw_self = _draw_terminal;
}
