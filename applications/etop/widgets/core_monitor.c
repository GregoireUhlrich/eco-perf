#include "core_monitor.h"
#include "eco_perf/cute_terminal/definitions/error.h"
#include <stdlib.h>

void _update_core_monitor(twidget_t *twidget);

const twidget_interface_t core_monitor_twidget_interface = {
    _update_core_monitor,
    default_twidget_draw,
    default_twidget_free};

void init_core_monitor_twidget_data(core_monitor_twidget_data_t *data)
{
    data->title[0] = '\0';
    data->core_data = NULL;
}

void init_core_monitor_twidget_config(core_monitor_twidget_config_t *config)
{
}

void init_core_monitor_tstack(core_monitor_tstack_t *stack)
{
    twidget_t *widget = &stack->twidget;
    core_monitor_twidget_data_t *data = &stack->data;
    core_monitor_twidget_config_t *config = &stack->config;
    init_core_monitor_twidget_data(data);
    init_core_monitor_twidget_config(config);
    twidget_init(widget);
    widget->interface = &core_monitor_twidget_interface;
    widget->stack = (void *)stack;
    widget->size.y = 1;
    widget->size.x = 50;

    twidget_linear_layout_init(&stack->layout, CT_HORIZONTAL);
    stack->layout.config.horizontal_align_mode = CT_TOP_OR_LEFT;
    twidget_set_layout(widget, &stack->layout);

    text_line_tstack_init(&stack->title);
    percent_bar_tstack_init(&stack->percent_bar);

    twidget_add_child(widget, &stack->title.twidget);
    twidget_add_child(widget, &stack->percent_bar.twidget);
}

void free_core_monitor_stack(core_monitor_tstack_t *monitor)
{
    twidget_free(&monitor->twidget);
}

void set_core_monitor_data(
    core_monitor_tstack_t *stack,
    cpu_core_data_t *core_data)
{
    stack->data.core_data = core_data;
}

void set_core_monitor_title(
    core_monitor_tstack_t *stack,
    char const *title)
{
    int title_length = 0;
    char *core_title = stack->data.title;
    while (*title && title_length < MAX_CORE_MONITOR_TITLE_SIZE)
    {
        *core_title = *title;
        ++title;
        ++core_title;
    }
    *core_title = '\0';
    text_line_set_content(&stack->title, stack->data.title);
}

void _update_core_monitor(twidget_t *twidget)
{
    core_monitor_tstack_t *stack = (core_monitor_tstack_t *)twidget->stack;
    core_monitor_twidget_data_t *data = &stack->data;
    percent_bar_data_t *percent_bar_data = &stack->percent_bar.data;
    percent_bar_data_init(percent_bar_data);
    percent_data_add(
        percent_bar_data,
        data->core_data->user_ratio, CT_RED);
    percent_data_add(
        percent_bar_data,
        data->core_data->sys_ratio, CT_GREEN);
    percent_data_add(
        percent_bar_data,
        data->core_data->nice_ratio, CT_BLUE);

    if (!stack->data.title[0])
    {
        stack->layout.config.spacing = 0;
    }
    else
    {
        stack->layout.config.spacing = 1;
    }
}
