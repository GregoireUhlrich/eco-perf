#include "term_layouts.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

term_layout_config_t get_default_term_layout_config()
{
    term_layout_config_t config;
    config.align_mode = TERM_LAYOUT_TOPLEFT;
    config.auto_children_resize = 1;
    return config;
}

void init_term_hlayout(
    term_hlayout_t *layout,
    term_layout_config_t *config)
{
    init_term_drawable(layout);
    layout->config = (void *)config;
    layout->update = update_term_hlayout;
}

void init_term_vlayout(
    term_vlayout_t *layout,
    term_layout_config_t *config)
{
    init_term_drawable(layout);
    layout->config = (void *)config;
    layout->update = update_term_vlayout;
}

void _find_strechable_elements(
    term_drawable_t *layout,
    int *size_to_divide,
    int *n_stretchables,
    int stretchables[],
    int offset)
{
    *size_to_divide = layout->size_v[offset];
    *n_stretchables = 0;
    for (int i = 0; i != layout->n_children; ++i)
    {
        if (layout->children[i]->fixed_size_v[offset])
        {
            *size_to_divide -= layout->children[i]->size_v[offset];
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
    term_drawable_t *layout,
    int size_to_divide,
    int n_stretchables,
    int stretchables[],
    int offset)
{
    term_layout_config_t *config = (term_layout_config_t *)layout->config;
    if (!config->auto_children_resize)
    {
        return;
    }
    int step_size = size_to_divide / n_stretchables;
    int last_step = size_to_divide - (n_stretchables - 1) * step_size;

    for (int i = 0; i != n_stretchables - 1; ++i)
    {
        layout->children[stretchables[i]]->size_v[offset] = step_size;
    }
    layout->children[stretchables[n_stretchables - 1]]->size_v[offset] = last_step;

    // Put perpendicular size equal
    for (int i = 0; i != layout->n_children; ++i)
    {
        layout->children[i]->size_v[1 - offset] = layout->size_v[1 - offset];
    }
}

void _apply_pos_update(
    term_drawable_t *layout,
    int offset)
{
    term_layout_config_t *config = (term_layout_config_t *)layout->config;

    // Alignement in the layout direction
    int current_pos = layout->pos_v[offset];
    for (int i = 0; i != layout->n_children; ++i)
    {
        layout->children[i]->pos_v[offset] = current_pos;
        current_pos += layout->children[i]->size_v[offset];
    }

    // Alignement perpendicular to the layout
    int anti_offset = 1 - offset;
    const int parent_size = layout->size_v[anti_offset];
    for (int i = 0; i != layout->n_children; ++i)
    {
        unsigned int new_pos = 0; // relative pos
        if (config->align_mode != TERM_LAYOUT_TOPLEFT)
        {
            unsigned int child_size = layout->children[i]->size_v[anti_offset];
            int size_diff = parent_size - child_size;
            if (config->align_mode == TERM_LAYOUT_CENTER)
                new_pos += size_diff / 2;
            else
                new_pos += size_diff;
        }
        layout->children[i]->pos_v[anti_offset] = new_pos;
    }
}

void _update_term_layout(term_drawable_t *layout, int hlayout)
{
    const int offset = !hlayout;
    int n_stretchables;
    int size_to_divide;
    int stretchables[MAX_TERM_N_LAYOUT_ELEMENTS];
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

void update_term_hlayout(term_hlayout_t *layout)
{
    _update_term_layout(layout, 1);
}

void update_term_vlayout(term_vlayout_t *layout)
{
    _update_term_layout(layout, 0);
}
