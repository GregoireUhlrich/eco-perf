#ifndef ECO_PERF_TERM_LAYOUT_H_INCLUDED
#define ECO_PERF_TERM_LAYOUT_H_INCLUDED

#include "term_drawable.h"

#define MAX_TERM_N_LAYOUT_ELEMENTS 15

typedef term_drawable_t term_hlayout_t;

typedef term_drawable_t term_vlayout_t;

typedef enum TermLayoutAlignMode
{
    TERM_LAYOUT_TOPLEFT,
    TERM_LAYOUT_CENTER,
    TERM_LAYOUT_BOTTOMRIGHT
} term_layout_align_mode_t;

typedef struct TermLayoutConfig
{
    term_layout_align_mode_t align_mode;
    int auto_children_resize;
} term_layout_config_t;

term_layout_config_t get_default_term_layout_config();

void init_term_hlayout(
    term_hlayout_t *layout,
    term_layout_config_t *config);

void init_term_vlayout(
    term_vlayout_t *layout,
    term_layout_config_t *config);

void update_term_hlayout(term_hlayout_t *layout);

void update_term_vlayout(term_vlayout_t *layout);

#endif
