#ifndef CUTE_TERMINAL_TWIDGET_INTERFACE_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_INTERFACE_H_INCLUDED

struct TWidget;

typedef void (*twidget_update_function_t)(struct TWidget *);
typedef void (*twidget_draw_function_t)(struct TWidget *);
typedef void (*twidget_free_function_t)(struct TWidget *);

typedef struct TWidgetInterface
{

    twidget_update_function_t update;
    twidget_draw_function_t draw;
    twidget_free_function_t free;

} twidget_interface_t;

void default_twidget_update(struct TWidget *widget);
void default_twidget_draw(struct TWidget *widget);
void default_twidget_free(struct TWidget *widget);

#endif
