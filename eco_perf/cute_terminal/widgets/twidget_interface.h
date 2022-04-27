#ifndef CUTE_TERMINAL_TWIDGET_INTERFACE_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_INTERFACE_H_INCLUDED

struct TWidget;

typedef struct TWidgetInterface
{

    void (*update)(struct TWidget *twidget);

    void (*draw)(struct TWidget *twidget);

    void (*release)(struct TWidget *twidget);

} twidget_interface_t;

#endif
