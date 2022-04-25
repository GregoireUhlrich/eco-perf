#ifndef ECO_PERF_TWIDGET_LAYOUT_H_INCLUDED
#define ECO_PERF_TWIDGET_LAYOUT_H_INCLUDED

struct TWidget;

typedef enum LayoutAlignMode
{
    LAYOUT_TWIDGET_TOPLEFT,
    LAYOUT_TWIDGET_BOTTOMRIGHT,
    LAYOUT_TWIDGET_CENTER,
} layout_align_mode_t;

typedef struct TWidgetLayoutConfig
{
    int auto_children_resize;
    layout_align_mode_t horizontal_align_mode;
    layout_align_mode_t vertical_align_mode;
} twidget_layout_config_t;

typedef struct TWidgetLayout
{
    twidget_layout_config_t config;

    void (*apply_layout)(
        struct TWidgetLayout const *layout,
        struct TWidget *widget);
} twidget_layout_t;

twidget_layout_config_t default_twidget_layout_config();

void init_twidget_layout(twidget_layout_t *layout);

twidget_layout_t default_twidget_layout();

#endif
