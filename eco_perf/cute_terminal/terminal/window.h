#ifndef ECO_PERF_TERMINAL_WINDOW_H_INCLUDED
#define ECO_PERF_TERMINAL_WINDOW_H_INCLUDED

typedef struct TerminalWindow
{
    unsigned int width;
    unsigned int height;
} terminal_window_t;

void init_terminal_window(terminal_window_t *data);

#endif
