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
    union
    {
        term_vector_t pos;
        unsigned int pos_v[2];
    };
    union
    {
        term_vector_t size;
        unsigned int size_v[2];
    };
    union
    {
        term_vector_t fixed_size;
        unsigned int fixed_size_v[2];
    };

    int n_children;
    int _memory_size;
    struct TermDrawable **children;

    void *config; // possible additional config
    void *data;   // possible additional data

    void (*update)(struct TermDrawable *drawable);

    term_vector_t (*get_origin)(
        struct TermDrawable const *drawable);

    int (*draw_self)(struct TermDrawable const *drawable);
} term_drawable_t;

void init_term_vector(term_vector_t *vector);
void init_term_drawable(term_drawable_t *drawable);

term_vector_t get_default_term_origin();

void update_term_drawable(term_drawable_t *drawable);

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
