#ifndef CUTE_TERMINAL_TERMINAL_WINDOW_H_INCLUDED
#define CUTE_TERMINAL_TERMINAL_WINDOW_H_INCLUDED

typedef struct TerminalWindow
{
    unsigned int width;
    unsigned int height;
} terminal_window_t;

void terminal_window_init(terminal_window_t *data);

#endif
