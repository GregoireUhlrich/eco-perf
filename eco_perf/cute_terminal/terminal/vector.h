#ifndef CUTE_TERMINAL_TERMINAL_VECTOR_H_INCLUDED
#define CUTE_TERMINAL_TERMINAL_VECTOR_H_INCLUDED

typedef struct TerminalVector
{
    unsigned int x;
    unsigned int y;
} terminal_vector_t;

void terminal_vector_init(terminal_vector_t *vector);

void terminal_vector_set(
    terminal_vector_t *vector,
    unsigned int x,
    unsigned int y);

terminal_vector_t terminal_vector_create();

#endif
