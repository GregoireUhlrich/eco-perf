#include "box.h"
#include "../io/io.h"
#include "../terminal/cursor.h"
#include <stdio.h>

term_vector_t _get_box_origin(term_box_t const *box)
{
    static const term_vector_t origin = {1, 1};
    return origin;
}

void _new_box_line(int line_size)
{
    move_cursor_down(1);
    move_cursor_left(line_size);
}

void _set_box_cursor_pos(term_vector_t box_pos)
{
    move_cursor_right(box_pos.x);
    move_cursor_down(box_pos.y);
}

void _reset_box_cursor_pos(
    term_vector_t box_pos,
    term_vector_t box_size)
{
    move_cursor_left(box_pos.x + box_size.x);
    move_cursor_up(box_pos.y + box_size.y - 1);
}

int _draw_box(term_box_t const *box)
{
    char background = ' ';
    if (box->config)
    {
        background = ((box_twidget_t *)box->config)->background;
    }
    _set_box_cursor_pos(box->pos);
    const int lx = box->size.x;
    const int ly = box->size.y;
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
    _reset_box_cursor_pos(box->pos, box->size);

    return 1;
}

void init_box_twidget(
    term_box_t *box,
    term_vector_t pos,
    term_vector_t size)
{
    init_twidget(box);
    box->pos = pos;
    box->size = size;
    box->draw_self = _draw_box;
    box->get_origin = _get_box_origin;
}

void set_box_twidget_config(
    term_box_t *box,
    box_twidget_t *config)
{
    box->config = (void *)config;
}
