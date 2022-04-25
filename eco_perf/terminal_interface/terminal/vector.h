#ifndef ECO_PERF_TERMINAL_VECTOR_H_INCLUDED
#define ECO_PERF_TERMINAL_VECTOR_H_INCLUDED

typedef struct TerminalVector
{
    unsigned int x;
    unsigned int y;
} terminal_vector_t;

void init_terminal_vector(terminal_vector_t *vector);

void set_terminal_vector(
    terminal_vector_t *vector,
    unsigned int x,
    unsigned int y);

terminal_vector_t get_terminal_vector();

#endif
