#include "vector.h"

void init_terminal_vector(terminal_vector_t *vector)
{
    set_terminal_vector(vector, 0, 0);
}

void set_terminal_vector(
    terminal_vector_t *vector,
    unsigned int x,
    unsigned int y)
{
    vector->x = x;
    vector->y = y;
}

terminal_vector_t get_terminal_vector()
{
    terminal_vector_t vector;
    init_terminal_vector(&vector);
    return vector;
}
