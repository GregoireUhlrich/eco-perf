#include "terminal.h"
#include "../definitions/error.h"
#include "../terminal/cursor.h"
#include "../terminal/window.h"

void _update_terminal(twidget_t *terminal);
void _draw_terminal(twidget_t *terminal);

const twidget_interface_t terminal_twidget_interface = {
    _update_terminal,
    _draw_terminal,
    default_twidget_free};

void init_terminal_tstack(
    terminal_tstack_t *terminal,
    twidget_t *main_twidget)
{
    twidget_t *term_twidget = &terminal->terminal_container_twidget;
    init_twidget(term_twidget);
    term_twidget->interface = &terminal_twidget_interface;
    add_twidget_child(term_twidget, main_twidget);
}

void _update_terminal(twidget_t *terminal)
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

void _draw_terminal(twidget_t *terminal)
{
    clear_terminal();
}
