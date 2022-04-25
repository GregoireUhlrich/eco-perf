#ifndef ECO_PERF_TERM_BOX_H_INCLUDED
#define ECO_PERF_TERM_BOX_H_INCLUDED

#include "twidget.h"

typedef twidget_t term_box_t;

typedef struct TermBoxConfig
{
    char background;
} term_box_config_t;

void init_term_box(
    term_box_t *box,
    term_vector_t pos,
    term_vector_t size);

void set_config(
    term_box_t *box,
    term_box_config_t *config);

#endif
