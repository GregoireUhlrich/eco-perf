#include "proc_monitor.h"

void _update_proc_monitor(twidget_t *twidget);
void _draw_proc_monitor(twidget_t *twidget);
void _free_proc_monitor(twidget_t *twidget);

const twidget_interface_t proc_monitor_twidget_interface = {
    _update_proc_monitor,
    _draw_proc_monitor,
    _free_proc_monitor};

void init_proc_monitor_twidget_data(proc_monitor_twidget_data_t *monitor)
{
    process_list_init(&monitor->process_list);
}

void init_proc_monitor_twidget_config(proc_monitor_twidget_config_t *monitor)
{
}

void init_proc_monitor_tstack(proc_monitor_tstack_t *stack)
{
    twidget_init(&stack->twidget);
    init_proc_monitor_twidget_config(&stack->config);
    init_proc_monitor_twidget_data(&stack->data);
}

void _update_proc_monitor(twidget_t *twidget)
{
}

void _draw_proc_monitor(twidget_t *twidget)
{
}

void _free_proc_monitor(twidget_t *twidget)
{
}
