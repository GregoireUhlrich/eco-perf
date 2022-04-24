#ifndef ECO_PERF_TERM_DRAWABLE_H_INCLUDED
#define ECO_PERF_TERM_DRAWABLE_H_INCLUDED

typedef struct TermVector
{
    unsigned int x;
    unsigned int y;
} term_vector_t;

typedef struct TermDrawable
{
    int hidden;
    term_vector_t top_left;
    term_vector_t size;

    int n_children;
    int _memory_size;
    struct TermDrawable **children;

    void (*parent_update)(
        struct TermDrawable const *drawable,
        struct TermDrawable const *parent);
    int (*draw_self)(
        struct TermDrawable const *drawable);
} term_drawable_t;

void init_term_vector(term_vector_t *vector);
void init_term_drawable(term_drawable_t *drawable);

int draw_term_drawable(term_drawable_t *drawable);

void add_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child);

void free_term_drawable(term_drawable_t *drawable);

#endif
