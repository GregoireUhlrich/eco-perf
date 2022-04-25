#ifndef ECO_PERF_TERMINAL_CURSOR_H_INCLUDED
#define ECO_PERF_TERMINAL_CURSOR_H_INCLUDED

void save_cursor_position();
void restore_cursor_position();

void set_cursor_position(int i_line, int i_column);

void move_cursor_left(int n_columns);
void move_cursor_right(int n_columns);
void move_cursor_down(int n_lines);
void move_cursor_up(int n_lines);

void clear_lines_up(int n_lines);

void clear_end_line();
void clear_terminal();

#endif
