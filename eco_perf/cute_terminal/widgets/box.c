#include "box.h"
#include "../io/io.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

void _draw_box(twidget_t *box);

const twidget_interface_t box_twidget_interface = {
    default_twidget_update,
    _draw_box,
    default_twidget_free};

void box_tstack_init(
    box_tstack_t *box)
{
    twidget_init(&box->twidget);
    box->config.background = ' ';
    box->twidget.stack = (void *)box;
    box->twidget.interface = &box_twidget_interface;
}

box_tstack_t box_tstack_create()
{
    box_tstack_t stack;
    box_tstack_init(&stack);
    return stack;
}

void _new_box_line(int line_size)
{
    move_cursor_down(1);
    move_cursor_left(line_size);
}

void _set_box_cursor_pos(terminal_vector_t box_pos)
{
    // move to display the external border
    move_cursor_left(1);
    move_cursor_up(1);
}

void _reset_box_cursor_pos(terminal_vector_t box_size)
{
    // move to display the external border
    move_cursor_left(box_size.x + 1);
    move_cursor_up(box_size.y);
}

void _draw_box(twidget_t *box)
{
    char background = ' ';
    box_twidget_config_t *config = &((box_tstack_t *)box->stack)->config;
    if (config)
    {
        background = config->background;
    }
    const int lx = box->size.x + 2; // 2 = external border
    const int ly = box->size.y + 2; // 2 = external border
    char buffer[500];
    fill_str(buffer, '-', lx);
    printf("%s", buffer);
    _new_box_line(lx);
    for (int i = 0; i < ly - 2; ++i)
    {
        fill_str(buffer, background, lx);
        buffer[0] = '|';
        buffer[lx - 1] = '|';
        printf("%s", buffer);
        _new_box_line(lx);
    }
    fill_str(buffer, '-', lx);
    printf("%s", buffer);
    _reset_box_cursor_pos(box->size);
}
