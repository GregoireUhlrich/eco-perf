#include "cpu_info.h"
#include <stdlib.h>

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
    data->cpu_data = NULL;
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
    cpu_core_data_t const *core_data = data->cpu_data;
    init_percent_bar_data(bar_data);
    add_percent_data(bar_data, core_data->user_ratio, CT_GREEN);
    add_percent_data(bar_data, core_data->sys_ratio, CT_BLUE);
    add_percent_data(bar_data, core_data->nice_ratio, CT_YELLOW);
}
