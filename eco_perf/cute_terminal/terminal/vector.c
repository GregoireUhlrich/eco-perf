#include "vector.h"

void terminal_vector_init(terminal_vector_t *vector)
{
    terminal_vector_set(vector, 0, 0);
}

void terminal_vector_set(
    terminal_vector_t *vector,
    unsigned int x,
    unsigned int y)
{
    vector->x = x;
    vector->y = y;
}

terminal_vector_t terminal_vector_create()
{
    terminal_vector_t vector;
    terminal_vector_init(&vector);
    return vector;
}
