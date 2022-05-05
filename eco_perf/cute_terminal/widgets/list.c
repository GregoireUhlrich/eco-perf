#include "list.h"

void _update_list(twidget_t *twidget);

const twidget_interface_t list_twidget_interface = {
    _update_list,
    default_twidget_draw,
    default_twidget_free};

void init_list_tstack_t(list_tstack_t *list)
{
    twidget_init(&list->twidget);
    list->line_height = 1;
    list->line_spacing = 0;
    twidget_linear_layout_init(&list->layout, CT_VERTICAL);
    twidget_set_layout(&list->twidget, &list->layout);
}

list_tstack_t list_tstack_create()
{
    list_tstack_t stack;
    list_tstack_init(&stack);
    return stack;
}

void _update_list(twidget_t *twidget)
{
    list_tstack_t *stack = (list_tstack_t *)twidget->stack;
    twidget->layout->config.spacing = stack->line_spacing;
    for (int i = 0; i != twidget->children.size; ++i)
    {
        twidget_t *child = twidget->children.data[i];
        child->size.y = stack->line_height;
        child->fixed_size.y = 1;
    }
}
