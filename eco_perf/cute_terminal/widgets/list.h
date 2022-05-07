#ifndef ECO_PERF_LIST_TWIDGET_H_INCLUDED
#define ECO_PERF_LIST_TWIDGET_H_INCLUDED

#include "layouts/linear_layout.h"
#include "twidget.h"

typedef struct ListTwidgetConfig
{
    ct_direction_t layout_direction;
    es_size_t line_height;
    es_size_t line_spacing;
} list_twidget_config_t;
typedef struct ListTStack
{
    twidget_t twidget;
    twidget_linear_layout_t layout;
    list_twidget_config_t config;
} list_tstack_t;

extern const twidget_interface_t list_twidget_interface;

void list_tstack_init(list_tstack_t *list);

list_tstack_t list_tstack_create();

void list_tstack_set_direction(
    list_tstack_t *list,
    ct_direction_t direction);

void list_tstack_set_alignement(
    list_tstack_t *list,
    ct_alignement_policy_t alignement);

#endif
