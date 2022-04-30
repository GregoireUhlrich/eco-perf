#include "eco_perf/cute_terminal/terminal/cursor.h"
#include "eco_perf/cute_terminal/widgets/box.h"
#include "eco_perf/cute_terminal/widgets/twidget.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

void print_drawable(twidget_t *drawable)
{
    printf("Drawable with %d children (mem_size=%d)\n",
           drawable->children.size,
           drawable->children._memory_size);
    for (int i = 0; i != drawable->children.size; ++i)
    {
        printf(" -> child %d: ", i);
        print_drawable(drawable->children.widgets[i]);
    }
}

void test_boxes()
{
    box_tstack_t bigger_box;
    terminal_vector_t pos, size;
    pos.x = 10;
    pos.y = 5;
    size.x = 100;
    size.y = 10;
    init_box_tstack(&bigger_box);
    bigger_box.twidget.pos = pos;
    bigger_box.twidget.size = size;

    clear_terminal();
    box_tstack_t box1, box2, box3;
    init_box_tstack(&box1);
    init_box_tstack(&box2);
    init_box_tstack(&box3);

    pos.x = 0;
    pos.y = 1;
    size.x = 5;
    size.y = 5;
    box1.twidget.pos = pos;
    box1.twidget.size = size;

    pos.x += 10;
    size.x += 2;
    box2.twidget.pos = pos;
    box2.twidget.size = size;
    box2.config.background = 'x';

    pos.x = 3;
    pos.y = 4;
    size.x = 20;
    size.y = 8;
    box3.twidget.pos = pos;
    box3.twidget.size = size;
    box3.config.background = '*';

    add_twidget_child(&bigger_box.twidget, &box1.twidget);
    add_twidget_child(&bigger_box.twidget, &box2.twidget);
    add_twidget_child(&bigger_box.twidget, &box3.twidget);

    draw_twidget(&bigger_box.twidget);
    char c;
    scanf("%c", &c);
}

int main(int argc, char const *argv[])
{
    twidget_t terminal;
    init_twidget(&terminal);

    twidget_t panel1, panel2, panel3;
    init_twidget(&panel1);
    init_twidget(&panel2);
    init_twidget(&panel3);

    twidget_t element1, element2;
    init_twidget(&element1);
    init_twidget(&element2);

    add_twidget_child(&panel1, &element1);
    add_twidget_child(&panel1, &element2);

    add_twidget_child(&terminal, &panel1);
    add_twidget_child(&terminal, &panel2);
    add_twidget_child(&terminal, &panel3);

    print_drawable(&element1);
    print_drawable(&panel1);
    print_drawable(&terminal);

    remove_twidget_child(&panel1, &element2, 1);
    remove_twidget_child(&terminal, &panel1, 1);
    print_drawable(&terminal);

    free_twidget(&terminal);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf("lines %d\n", w.ws_row);
    printf("columns %d\n", w.ws_col);

    if (argc > 1 && atoi(argv[1]))
    {
        test_boxes();
    }

    return 0;
}
