#ifndef ECO_PERF_TERM_LAYOUT_H_INCLUDED
#define ECO_PERF_TERM_LAYOUT_H_INCLUDED

#include "twidget.h"

#define MAX_TERM_N_LAYOUT_ELEMENTS 15

typedef twidget_t hlayout_twidget_t;

typedef twidget_t vlayout_twidget_t;

typedef enum LayoutTWidgetAlignMode
{
    TERM_LAYOUT_TOPLEFT,
    TERM_LAYOUT_CENTER,
    TERM_LAYOUT_BOTTOMRIGHT
} layout_t_widget_align_mode_t;

typedef struct LayoutTWidgetConfig
{
    layout_t_widget_align_mode_t align_mode;
    int auto_children_resize;
} layout_twidget_config_t;

layout_twidget_config_t get_default_layout_twidget_config();

void init_hlayout_twidget(
    hlayout_twidget_t *layout,
    layout_twidget_config_t *config);

void init_vlayout_twidget(
    vlayout_twidget_t *layout,
    layout_twidget_config_t *config);

void update_hlayout_twidget(hlayout_twidget_t *layout);

void update_vlayout_twidget(vlayout_twidget_t *layout);

#endif
