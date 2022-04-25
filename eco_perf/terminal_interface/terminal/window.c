#include "window.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void init_terminal_window(terminal_window_t *data)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    data->width = w.ws_col;
    data->height = w.ws_row;
}
