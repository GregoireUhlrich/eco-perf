#include "terminal_panel.h"
#include "../terminal/cursor.h"
#include "../terminal/window.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void _update_terminal(terminal_panel_t *terminal)
{
    terminal_window_t terminal_window;
    init_terminal_window(&terminal_window);
    terminal->size.x = terminal_window.width;
    terminal->size.y = terminal_window.height - 1;
    terminal->pos.x = 0;
    terminal->pos.y = 1;
    if (terminal->n_children == 1)
    {
        terminal->children[0]->size = terminal->size;
    }
    else
    {
        errno = EINVAL;
        perror("Terminal should have one and one drawable child only.");
        exit(1);
    }
}

int _draw_terminal(terminal_panel_t const *terminal)
{
    clear_terminal();
    return 1;
}

void init_terminal_panel(
    terminal_panel_t *terminal,
    term_drawable_t *main_drawable)
{
    init_term_drawable(terminal);
    add_term_drawable_child(terminal, main_drawable);
    terminal->update = _update_terminal;
    terminal->update(terminal);
    terminal->draw_self = _draw_terminal;
}
