#include "terminal_cursor.h"
#include <stdio.h>

void save_cursor_position()
{
    printf("\033[s");
}

void restore_cursor_position()
{
    printf("\033[u");
}

void set_cursor_position(int i_line, int i_column)
{
    printf("\033[%d;%dH", i_line, i_column);
}

void move_cursor_left(int n_columns)
{
    printf("\033%dD", n_columns);
}

void move_cursor_right(int n_columns)
{
    printf("\033%dC", n_columns);
}

void move_cursor_down(int n_lines)
{
    printf("\033[%dB", n_lines);
}

void move_cursor_up(int n_lines)
{
    printf("\033[%dA", n_lines);
}

void clear_end_line()
{
    printf("\033[K");
}

void clear_terminal()
{
    printf("\033c");
}

void clear_lines_up(int n_lines)
{
    for (int i = 0; i != n_lines; ++i)
    {
        move_cursor_up(1);
        clear_end_line();
    }
}
