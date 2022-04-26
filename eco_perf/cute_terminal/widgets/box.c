#include "box.h"
#include "../io/io.h"
#include "../io/string_utils.h"
#include "../terminal/cursor.h"
#include <stdio.h>

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

int _draw_box(box_twidget_t const *box)
{
    char background = ' ';
    if (box->config)
    {
        background = ((box_twidget_config_t *)box->config)->background;
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

    return 1;
}

void init_box_twidget(
    box_twidget_t *box)
{
    init_twidget(box);
    box->draw_self = _draw_box;
}

void set_box_twidget_config(
    box_twidget_t *box,
    box_twidget_config_t *config)
{
    box->config = (void *)config;
}
