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
        print_drawable(drawable->children.data[i]);
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
    box_tstack_init(&bigger_box);
    bigger_box.twidget.pos = pos;
    bigger_box.twidget.size = size;

    clear_terminal();
    box_tstack_t box1, box2, box3;
    box_tstack_init(&box1);
    box_tstack_init(&box2);
    box_tstack_init(&box3);

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

    twidget_add_child(&bigger_box.twidget, &box1.twidget);
    twidget_add_child(&bigger_box.twidget, &box2.twidget);
    twidget_add_child(&bigger_box.twidget, &box3.twidget);

    twidget_draw(&bigger_box.twidget);
    char c;
    scanf("%c", &c);
}

int main(int argc, char const *argv[])
{
    twidget_t terminal;
    twidget_init(&terminal);

    twidget_t panel1, panel2, panel3;
    twidget_init(&panel1);
    twidget_init(&panel2);
    twidget_init(&panel3);

    twidget_t element1, element2;
    twidget_init(&element1);
    twidget_init(&element2);

    twidget_add_child(&panel1, &element1);
    twidget_add_child(&panel1, &element2);

    twidget_add_child(&terminal, &panel1);
    twidget_add_child(&terminal, &panel2);
    twidget_add_child(&terminal, &panel3);

    print_drawable(&element1);
    print_drawable(&panel1);
    print_drawable(&terminal);

    twidget_remove_child(&panel1, &element2, 1);
    twidget_remove_child(&terminal, &panel1, 1);
    print_drawable(&terminal);

    twidget_free(&terminal);

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
