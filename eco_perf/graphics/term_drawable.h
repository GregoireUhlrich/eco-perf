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
    term_vector_t pos;
    term_vector_t size;
    term_vector_t stretch;

    int n_children;
    int _memory_size;
    struct TermDrawable **children;

    void *config;

    void (*parent_update)(
        struct TermDrawable *drawable,
        struct TermDrawable const *parent);

    term_vector_t (*get_origin)(
        struct TermDrawable const *drawable);

    int (*draw_self)(struct TermDrawable const *drawable);
} term_drawable_t;

void init_term_vector(term_vector_t *vector);
void init_term_drawable(term_drawable_t *drawable);

term_vector_t get_default_term_origin();

int draw_term_drawable(term_drawable_t *drawable);

void add_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child);

int index_of_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child);

void remove_term_drawable_child(
    term_drawable_t *parent,
    term_drawable_t *child,
    int release_child);

void free_term_drawable(term_drawable_t *drawable);

#endif
