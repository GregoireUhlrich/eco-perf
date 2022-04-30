#include "cpu_monitor.h"
#include "eco_perf/cute_terminal/definitions/error.h"
#include "eco_perf/cute_terminal/io/string_utils.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

void _update_cpu_monitor(twidget_t *twidget);
void _free_cpu_monitor(twidget_t *twidget);

const twidget_interface_t cpu_monitor_twidget_interface = {
    _update_cpu_monitor,
    default_twidget_draw,
    _free_cpu_monitor};

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
    monitor->n_core_monitors = 0;
    monitor->core_monitors = NULL;
    monitor->twidget.interface = &cpu_monitor_twidget_interface;
}

void free_cpu_monitor_tstack(cpu_monitor_tstack_t *monitor)
{
    free_twidget(&monitor->twidget);
}

void _ensure_core_monitor_size(
    cpu_monitor_tstack_t *monitor,
    int size);

void _set_core_monitor_data(cpu_monitor_tstack_t *monitor);

void _update_cpu_monitor_bounds(cpu_monitor_tstack_t *monitor);

void set_cpu_monitor_data(
    cpu_monitor_tstack_t *monitor,
    cpu_data_t *cpu_data)
{
    monitor->data.cpu_data = cpu_data;
    _ensure_core_monitor_size(
        monitor,
        cpu_data->n_cpus);
    _set_core_monitor_data(monitor);
    _update_cpu_monitor_bounds(monitor);
}

void _check_cpu_monitor_bounds(cpu_monitor_tstack_t *monitor);

void set_cpu_monitor_bounds(
    cpu_monitor_tstack_t *monitor,
    int first_core_monitor,
    int n_core_monitors)
{
    monitor->config.first_core_monitor = first_core_monitor;
    monitor->config.n_core_monitors = n_core_monitors;
    _check_cpu_monitor_bounds(monitor);
    _update_cpu_monitor_bounds(monitor);
}

void _update_cpu_monitor_bounds(cpu_monitor_tstack_t *monitor)
{
    const int n_cores = monitor->config.n_core_monitors;
    monitor->twidget.size.y = n_cores;
    if (n_cores > 0)
        monitor->twidget.size.x = monitor->core_monitors[0].twidget.size.x;
}

void _ensure_twidget_children_size(
    twidget_array_t *array,
    int n_children);

void _update_cpu_monitor(twidget_t *twidget)
{
    cpu_monitor_tstack_t *monitor = (cpu_monitor_tstack_t *)twidget->stack;
    _check_cpu_monitor_bounds(monitor);
    const int first_core = monitor->config.first_core_monitor;
    const int n_cores = monitor->config.n_core_monitors;
    _ensure_twidget_children_size(&twidget->children, n_cores);
    for (int i = 0; i != n_cores; ++i)
    {
        twidget->children.widgets[i] = &monitor->core_monitors[first_core + i].twidget;
    }
}

void _free_cpu_monitor(twidget_t *twidget)
{
    cpu_monitor_tstack_t *monitor = (cpu_monitor_tstack_t *)twidget->stack;
    free(monitor->core_monitors);
}

void _ensure_core_monitor_size(
    cpu_monitor_tstack_t *monitor,
    int size)
{
    if (size <= 0)
    {
        return;
    }
    const int old_size = monitor->n_core_monitors;
    core_monitor_tstack_t *core_monitors = monitor->core_monitors;
    if (old_size != size && core_monitors)
    {
        for (int i = 0; i != old_size; ++i)
            free_core_monitor_stack(&core_monitors[i]);
        free(core_monitors);
        core_monitors = NULL;
    }
    if (!core_monitors)
    {
        core_monitors = (core_monitor_tstack_t *)malloc(size * sizeof(core_monitor_tstack_t));
        for (int i = 0; i != size; ++i)
        {
            init_core_monitor_tstack(&core_monitors[i]);
        }
    }
    monitor->n_core_monitors = size;
    monitor->core_monitors = core_monitors;
}

void _print_cpu_title(char *destination, int cpu_index, int max_digits)
{
    char title[20];
    sprintf(title, "cpu #%d", cpu_index);
    rfill(destination, title, 5 + max_digits, ' ');
}

void _set_core_monitor_data(cpu_monitor_tstack_t *monitor)
{
    cpu_data_t *cpu_data = monitor->data.cpu_data;
    const int last_cpu_number = monitor->config.first_core_monitor + monitor->config.n_core_monitors;
    int max_digits = 1 + (int)log10f((float)last_cpu_number);
    for (int i = 0; i != monitor->n_core_monitors; ++i)
    {
        char title[20];
        _print_cpu_title(title, i, max_digits);
        set_core_monitor_data(&monitor->core_monitors[i], &cpu_data->core_data[i]);
        set_core_monitor_title(&monitor->core_monitors[i], title);
    }
}

void _check_cpu_monitor_bounds(cpu_monitor_tstack_t *monitor)
{
    cpu_monitor_twidget_config_t *config = &monitor->config;
    cpu_data_t *cpu_data = monitor->data.cpu_data;
    if (config->first_core_monitor < 0)
    {
        config->first_core_monitor = 0;
    }
    if (config->n_core_monitors < 0)
    {
        config->n_core_monitors = cpu_data->n_cpus - config->first_core_monitor;
    }
    const unsigned int first_core = config->first_core_monitor;
    const unsigned int last_core = config->first_core_monitor + config->n_core_monitors;
    const unsigned int n_cores = cpu_data->n_cpus;
    CT_ASSERT(
        (first_core < last_core) && (last_core <= n_cores),
        CT_VALUE_ERROR,
        "Bad cpu monitor bounds (first_core=%d, last_core=%d) for cpu data"
        " with %d cores.",
        first_core,
        last_core,
        cpu_data->n_cpus)
}

void _ensure_twidget_children_size(
    twidget_array_t *array,
    int n_children)
{
    resize_twidget_array(array, n_children);
}
