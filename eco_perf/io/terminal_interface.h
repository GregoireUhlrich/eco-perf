#ifndef ECO_PERF_TERMINAL_INTERFACE_H_INCLUDED
#define ECO_PERF_TERMINAL_INTERFACE_H_INCLUDED

typedef struct TerminalData
{
    unsigned int width;
    unsigned int height;
} terminal_data_t;

void fill_terminal_data(terminal_data_t *data);

#endif
