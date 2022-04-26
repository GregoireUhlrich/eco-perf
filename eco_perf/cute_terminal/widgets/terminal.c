#include "terminal.h"
#include "../definitions/error.h"
#include "../terminal/cursor.h"
#include "../terminal/window.h"

void _update_terminal(terminal_twidget_t *terminal)
{
    terminal_window_t terminal_window;
    init_terminal_window(&terminal_window);
    terminal->size.x = terminal_window.width - 3;
    terminal->size.y = terminal_window.height - 3;
    terminal->pos.x = 1;
    terminal->pos.y = 1;
    CT_ASSERT(
        terminal->children.size == 1,
        CT_VALUE_ERROR,
        "Terminal should have exactly one child widget, found %d.",
        terminal->children.size)
    terminal->children.widgets[0]->size = terminal->size;
}

int _draw_terminal(terminal_twidget_t const *terminal)
{
    clear_terminal();
    return 1;
}

void init_terminal_twidget(
    terminal_twidget_t *terminal,
    twidget_t *main_widget)
{
    init_twidget(terminal);
    add_twidget_child(terminal, main_widget);
    terminal->update = _update_terminal;
    terminal->draw_self = _draw_terminal;

    terminal->update(terminal);
}
