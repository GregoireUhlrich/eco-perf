#include "cpu_monitor.h"
#include <stddef.h>

void _update_cpu_monitor(twidget_t *twidget);
void _draw_cpu_monitor(twidget_t *twidget);

const twidget_interface_t cpu_monitor_twidget_interface = {
    _update_cpu_monitor,
    _draw_cpu_monitor,
    default_twidget_free};

void init_cpu_monitor_twidget_data(cpu_monitor_twidget_data_t *data)
{
    data->cpu_data = NULL;
}

void init_cpu_monitor_twidget_config(cpu_monitor_twidget_config_t *config)
{
    config->first_core_monitor = -1;
    config->n_core_monitors = -1;
}

void init_cpu_monitor_tstack(cpu_monitor_tstack_t *monitor)
{
    init_twidget(&monitor->twidget);

    init_cpu_monitor_twidget_data(&monitor->data);
    init_cpu_monitor_twidget_config(&monitor->config);

    init_twidget_linear_layout(&monitor->layout, CT_VERTICAL);
    set_twidget_layout(&monitor->twidget, &monitor->layout);

    monitor->twidget.stack = (void *)monitor;
    monitor->core_monitors = NULL;
}

void _update_cpu_monitor(twidget_t *twidget)
{
}

void _draw_cpu_monitor(twidget_t *twidget)
{
}
