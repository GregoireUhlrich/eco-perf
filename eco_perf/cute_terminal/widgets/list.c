#include "list.h"

void _update_list(twidget_t *twidget);

const twidget_interface_t list_twidget_interface = {
    _update_list,
    default_twidget_draw,
    default_twidget_free};

void list_tstack_init(list_tstack_t *list)
{
    twidget_init(&list->twidget);
    list->config.line_height = 1;
    list->config.line_spacing = 0;
    list->twidget.interface = &list_twidget_interface;
    list->twidget.stack = list;
    list_tstack_set_direction(list, CT_VERTICAL);
}

list_tstack_t list_tstack_create()
{
    list_tstack_t stack;
    list_tstack_init(&stack);
    return stack;
}

void list_tstack_set_direction(
    list_tstack_t *list,
    ct_direction_t direction)
{
    twidget_linear_layout_init(&list->layout, direction);
    list->layout.config.vertical_align_mode = CT_TOP_OR_LEFT;
    list->layout.config.horizontal_align_mode = CT_TOP_OR_LEFT;
    list->config.layout_direction = direction;
    twidget_set_layout(&list->twidget, &list->layout);
}

void list_tstack_set_alignement(
    list_tstack_t *list,
    ct_alignement_policy_t alignement)
{
    if (list->config.layout_direction == CT_VERTICAL)
        list->layout.config.horizontal_align_mode = alignement;
    else
        list->layout.config.vertical_align_mode = alignement;
}

void _update_list(twidget_t *twidget)
{
    list_tstack_t *stack = (list_tstack_t *)twidget->stack;
    twidget->layout->config.spacing = stack->config.line_spacing;
    for (int i = 0; i != twidget->children.size; ++i)
    {
        twidget_t *child = twidget->children.data[i];
        child->size.y = stack->config.line_height;
        child->fixed_size.y = 1;
    }
}
