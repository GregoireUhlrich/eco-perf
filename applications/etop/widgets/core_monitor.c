#include "core_monitor.h"
#include "eco_perf/cute_terminal/definitions/error.h"
#include <stdlib.h>

void _update_cpu_monitor(twidget_t *twidget);

const twidget_interface_t core_monitor_twidget_interface = {
    _update_cpu_monitor,
    default_twidget_draw,
    default_twidget_free};

void init_core_monitor_twidget_data(core_monitor_twidget_data_t *data)
{
    data->title = "";
    data->core_data = NULL;
}

void init_core_monitor_twidget_config(core_monitor_twidget_config_t *config)
{
}

void init_core_monitor_tmanager(core_monitor_tmanager_t *manager)
{
    twidget_t *widget = &manager->twidget;
    core_monitor_twidget_data_t *data = &manager->data;
    core_monitor_twidget_config_t *config = &manager->config;
    init_core_monitor_twidget_data(data);
    init_core_monitor_twidget_config(config);
    init_twidget(widget);
    widget->interface = &core_monitor_twidget_interface;
    widget->manager = (void *)manager;
    widget->size.y = 1;
    widget->size.x = 50;

    init_twidget_linear_layout(&manager->layout, CT_HORIZONTAL);
    set_twidget_layout(widget, &manager->layout);

    init_text_line_tmanager(&manager->title);
    init_percent_bar_tmanager(&manager->percent_bar);

    add_twidget_child(widget, &manager->title.twidget);
    add_twidget_child(widget, &manager->percent_bar.twidget);
}

void set_core_monitor_data(
    core_monitor_tmanager_t *manager,
    cpu_core_data_t *core_data)
{
    manager->data.core_data = core_data;
}

void set_core_monitor_title(
    core_monitor_tmanager_t *manager,
    char const *title)
{
    set_text_line_content(&manager->title, title);
}

void _update_cpu_monitor(twidget_t *twidget)
{
    core_monitor_tmanager_t *manager = (core_monitor_tmanager_t *)twidget->manager;
    core_monitor_twidget_data_t *data = &manager->data;
    percent_bar_data_t *percent_bar_data = &manager->percent_bar.data;
    init_percent_bar_data(percent_bar_data);
    add_percent_data(
        percent_bar_data,
        data->core_data->user_ratio, CT_RED);
    add_percent_data(
        percent_bar_data,
        data->core_data->sys_ratio, CT_GREEN);
    add_percent_data(
        percent_bar_data,
        data->core_data->nice_ratio, CT_BLUE);

    if (!manager->data.title)
    {
        manager->layout.config.spacing = 0;
    }
    else
    {
        manager->layout.config.spacing = 1;
    }
}

/*
void _setup_cpu_monitor_config(
    core_monitor_twidget_config_t *config,
    cpu_data_t *core_data);

void _udpate_cpu_twidget_children(
    twidget_t *twidget,
    cpu_data_t *core_data,
    int first_cpu,
    int n_cpus);
void _setup_cpu_monitor_config(
    core_monitor_twidget_config_t *config,
    cpu_data_t *core_data)
{
    if (config->first_displayed_cpu < 0)
    {
        config->first_displayed_cpu = 0;
    }
    if (config->n_displayed_cpu < 0)
    {
        config->n_displayed_cpu = core_data->n_cpus - config->first_displayed_cpu;
    }
    int valid_config = (config->first_displayed_cpu < core_data->n_cpus);
    valid_config &= (config->first_displayed_cpu + config->n_displayed_cpu >= core_data->n_cpus);
    CT_ASSERT(valid_config,
              CT_VALUE_ERROR,
              "CPU config (display cpus %d-%d) is invalid for cpu data of size %d.",
              config->first_displayed_cpu,
              config->first_displayed_cpu + config->n_displayed_cpu,
              core_data->n_cpus)
}

void _udpate_cpu_twidget_children(
    twidget_t *twidget,
    cpu_data_t *core_data,
    int first_cpu,
    int n_cpus)
{
    if (twidget->children.size != n_cpus)
    {
        free_twidget_children(twidget);
        allocate_twidget_array(&twidget->children, n_cpus);
    }
}
*/
/*
void init_cpu_info_twidget(
    cpu_info_twidget_t *widget);

void init_cpu_info_twidget_data(
    cpu_info_twidget_data_t *data);

void init_cpu_info_twidget_config(
    cpu_info_twidget_config_t *config);

void _update_cpu_data_widget(cpu_info_twidget_t *widget);

void init_cpu_info_twidget_container(
    cpu_info_twidget_container_t *container)
{
    init_twidget(&container->widget);
    init_text_line_twidget(&container->title_widget, "");
    init_cpu_info_twidget(&container->cpu_widget);
}

void set_cpu_info_twidget_data(
    cpu_info_twidget_t *widget,
    cpu_core_data_t *data);

void init_cpu_info_twidget(
    cpu_info_twidget_t *widget)
{
    init_twidget(widget);
    widget->update = _update_cpu_data_widget;
    widget->size.x = 50;
    widget->size.y = 1;
    widget->fixed_size.y = 1;
}

void init_cpu_info_twidget_data(
    cpu_info_twidget_data_t *data)
{
    data->title = "";
    data->core_data = NULL;
    init_percent_bar_data(&data->percent_bar_data);
}

void init_cpu_info_twidget_config(
    cpu_info_twidget_config_t *config)
{
    init_twidget_linear_layout(&config->layout, CT_HORIZONTAL);
    config->layout.config.spacing = 1;
    init_percent_bar_config(&config->percent_bar_config);
}

void _update_cpu_data_widget(cpu_info_twidget_t *widget)
{
    cpu_info_twidget_data_t *data = (cpu_info_twidget_data_t *)(widget->data);
    percent_bar_data_t *bar_data = &data->percent_bar_data;
    cpu_core_data_t const *core_data = data->core_data;
    init_percent_bar_data(bar_data);
    add_percent_data(bar_data, core_data->user_ratio, CT_GREEN);
    add_percent_data(bar_data, core_data->sys_ratio, CT_BLUE);
    add_percent_data(bar_data, core_data->nice_ratio, CT_YELLOW);
}

*/
