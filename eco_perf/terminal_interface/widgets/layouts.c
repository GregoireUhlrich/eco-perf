#include "layouts.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

layout_twidget_config_t get_default_layout_twidget_config()
{
    layout_twidget_config_t config;
    config.align_mode = LAYOUT_TWIDGET_TOPLEFT;
    config.auto_children_resize = 1;
    return config;
}

void init_hlayout_twidget(
    hlayout_twidget_t *layout,
    layout_twidget_config_t *config)
{
    init_twidget(layout);
    layout->config = (void *)config;
    layout->update = update_hlayout_twidget;
}

void init_vlayout_twidget(
    vlayout_twidget_t *layout,
    layout_twidget_config_t *config)
{
    init_twidget(layout);
    layout->config = (void *)config;
    layout->update = update_vlayout_twidget;
}

void _find_strechable_elements(
    twidget_t *layout,
    int *size_to_divide,
    int *n_stretchables,
    int stretchables[],
    int offset)
{
    *size_to_divide = layout->size_v[offset];
    *n_stretchables = 0;
    twidget_array_t *widgets = &layout->children;
    for (int i = 0; i != widgets->size; ++i)
    {
        if (widgets->widgets[i]->fixed_size_v[offset])
        {
            *size_to_divide -= widgets->widgets[i]->size_v[offset];
        }
        else
        {
            stretchables[*n_stretchables] = i;
            ++*n_stretchables;
        }
    }
    if (*size_to_divide < *n_stretchables)
    {
        errno = EINVAL;
        perror("Layout too small to contain the elements.");
        exit(1);
    }
}

void _apply_size_update(
    twidget_t *layout,
    int size_to_divide,
    int n_stretchables,
    int stretchables[],
    int offset)
{
    layout_twidget_config_t *config = (layout_twidget_config_t *)layout->config;
    if (!config->auto_children_resize)
    {
        return;
    }
    int step_size = size_to_divide / n_stretchables;
    int last_step = size_to_divide - (n_stretchables - 1) * step_size;

    twidget_array_t *widgets = &layout->children;
    for (int i = 0; i != n_stretchables - 1; ++i)
    {
        widgets->widgets[stretchables[i]]->size_v[offset] = step_size;
    }
    widgets->widgets[stretchables[n_stretchables - 1]]->size_v[offset] = last_step;

    // Put perpendicular size equal
    for (int i = 0; i != widgets->size; ++i)
    {
        if (!widgets->widgets[i]->fixed_size_v[1 - offset])
        {
            widgets->widgets[i]->size_v[1 - offset] = layout->size_v[1 - offset];
        }
    }
}

void _apply_pos_update(
    twidget_t *layout,
    int offset)
{
    layout_twidget_config_t *config = (layout_twidget_config_t *)layout->config;

    // Alignement in the layout direction
    int current_pos = layout->pos_v[offset];

    twidget_array_t *widgets = &layout->children;
    for (int i = 0; i != widgets->size; ++i)
    {
        widgets->widgets[i]->pos_v[offset] = current_pos;
        current_pos += widgets->widgets[i]->size_v[offset];
    }

    // Alignement perpendicular to the layout
    int anti_offset = 1 - offset;
    const int parent_size = layout->size_v[anti_offset];
    for (int i = 0; i != widgets->size; ++i)
    {
        unsigned int new_pos = 0; // relative pos
        if (config->align_mode != LAYOUT_TWIDGET_TOPLEFT)
        {
            unsigned int child_size = widgets->widgets[i]->size_v[anti_offset];
            int size_diff = parent_size - child_size;
            if (config->align_mode == LAYOUT_TWIDGET_CENTER)
                new_pos += size_diff / 2;
            else
                new_pos += size_diff;
        }
        widgets->widgets[i]->pos_v[anti_offset] = new_pos;
    }
}

void _update_term_layout(twidget_t *layout, int hlayout)
{
    const int offset = !hlayout;
    int n_stretchables;
    int size_to_divide;
    int stretchables[MAX_N_LAYOUT_TWIDGET_ELEMENTS];
    _find_strechable_elements(
        layout,
        &size_to_divide,
        &n_stretchables,
        stretchables,
        offset);
    if (!n_stretchables)
    {
        return;
    }
    _apply_size_update(
        layout,
        size_to_divide,
        n_stretchables,
        stretchables,
        offset);
    _apply_pos_update(layout, offset);
}

void update_hlayout_twidget(hlayout_twidget_t *layout)
{
    _update_term_layout(layout, 1);
}

void update_vlayout_twidget(vlayout_twidget_t *layout)
{
    _update_term_layout(layout, 0);
}
