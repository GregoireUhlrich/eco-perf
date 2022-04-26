#ifndef CUTE_TERMINAL_TWIDGET_LAYOUT_H_INCLUDED
#define CUTE_TERMINAL_TWIDGET_LAYOUT_H_INCLUDED

#include "../../definitions/alignement_policy.h"

struct TWidget;

typedef struct TWidgetLayoutConfig
{
    int auto_children_resize;
    int spacing;
    ct_alignement_policy_t horizontal_align_mode;
    ct_alignement_policy_t vertical_align_mode;
} twidget_layout_config_t;

typedef struct TWidgetLayout
{
    twidget_layout_config_t config;

    void (*apply_layout)(
        struct TWidgetLayout const *layout,
        struct TWidget *widget);
} twidget_layout_t;

void init_twidget_layout(twidget_layout_t *layout);

void init_twidget_layout_config(twidget_layout_config_t *config);

twidget_layout_t default_twidget_layout();

#endif
