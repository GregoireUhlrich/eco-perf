#include "terminal_interface.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void fill_terminal_data(terminal_data_t *data)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    data->width = w.ws_col;
    data->height = w.ws_row;
}
