#include "eco_perf/graphics/term_drawable.h"
#include <stdio.h>

void print_drawable(term_drawable_t *drawable)
{
    printf("Drawable with %d children (mem_size=%d)\n",
           drawable->n_children,
           drawable->_memory_size);
    for (int i = 0; i != drawable->n_children; ++i)
    {
        printf(" -> child %d: ", i);
        print_drawable(drawable->children[i]);
    }
}

int main()
{
    term_drawable_t terminal;
    init_term_drawable(&terminal);

    term_drawable_t panel1, panel2, panel3;
    init_term_drawable(&panel1);
    init_term_drawable(&panel2);
    init_term_drawable(&panel3);

    term_drawable_t element1, element2;
    init_term_drawable(&element1);
    init_term_drawable(&element2);

    add_term_drawable_child(&panel1, &element1);
    add_term_drawable_child(&panel1, &element2);

    add_term_drawable_child(&terminal, &panel1);
    add_term_drawable_child(&terminal, &panel2);
    add_term_drawable_child(&terminal, &panel3);

    print_drawable(&element1);
    print_drawable(&panel1);
    print_drawable(&terminal);

    free_term_drawable(&terminal);

    return 0;
}
