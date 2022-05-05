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

void terminal_tstack_init(
    terminal_tstack_t *terminal,
    twidget_t *main_twidget)
{
    twidget_t *term_twidget = &terminal->terminal_container_twidget;
    twidget_init(term_twidget);
    term_twidget->interface = &terminal_twidget_interface;
    twidget_add_child(term_twidget, main_twidget);
}

terminal_tstack_t terminal_tstack_create(twidget_t *main_twidget)
{
    terminal_tstack_t stack;
    terminal_tstack_init(&stack, main_twidget);
    return stack;
}

void _update_terminal(twidget_t *terminal)
{
    terminal_window_t terminal_window;
    terminal_window_init(&terminal_window);
    terminal->size.x = terminal_window.width - 3;
    terminal->size.y = terminal_window.height - 3;
    terminal->pos.x = 1;
    terminal->pos.y = 1;
    CT_ASSERT(
        terminal->children.size == 1,
        CT_VALUE_ERROR,
        "Terminal should have exactly one child widget, found %d.",
        terminal->children.size)
    twidget_t *child = terminal->children.data[0];
    child->size = terminal->size;
}

void _draw_terminal(twidget_t *terminal)
{
    clear_terminal();
}
