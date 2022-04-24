#include "eco_perf/graphics/term_box.h"
#include "eco_perf/graphics/term_drawable.h"
#include "eco_perf/io/terminal_cursor.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

void test_boxes()
{
    term_box_t bigger_box;
    term_vector_t pos, size;
    pos.x = 10;
    pos.y = 5;
    size.x = 100;
    size.y = 100;
    init_term_box(&bigger_box, pos, size);

    clear_terminal();
    term_box_t box1, box2, box3;

    pos.x = 0;
    pos.y = 1;
    size.x = 5;
    size.y = 5;
    init_term_box(&box1, pos, size);

    pos.x += 10;
    size.x += 30;
    term_box_config_t config_box2;
    init_term_box(&box2, pos, size);
    config_box2.background = 'x';
    box2.config = (void *)(&config_box2);

    pos.x = 3;
    pos.y = 4;
    size.x = 20;
    size.y = 10;
    term_box_config_t config_box3;
    config_box3.background = '*';
    init_term_box(&box3, pos, size);
    box3.config = (void *)(&config_box3);

    add_term_drawable_child(&bigger_box, &box1);
    add_term_drawable_child(&bigger_box, &box2);
    add_term_drawable_child(&bigger_box, &box3);

    draw_term_drawable(&bigger_box);
    char c;
    scanf("%c", &c);
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

    remove_term_drawable_child(&panel1, &element2, 1);
    remove_term_drawable_child(&terminal, &panel1, 1);
    print_drawable(&terminal);

    free_term_drawable(&terminal);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf("lines %d\n", w.ws_row);
    printf("columns %d\n", w.ws_col);

    // test_boxes();

    return 0;
}
