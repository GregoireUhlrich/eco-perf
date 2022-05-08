#include "proc_monitor.h"
#include "eco_perf/cute_terminal/io/format_definitions.h"
#include "eco_perf/cute_terminal/io/string_utils.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void _update_proc_monitor(twidget_t *twidget);
void _draw_proc_monitor(twidget_t *twidget);
void _free_proc_monitor(twidget_t *twidget);

const twidget_interface_t proc_monitor_twidget_interface = {
    _update_proc_monitor,
    _draw_proc_monitor,
    _free_proc_monitor};

void proc_monitor_tstack_init(proc_monitor_tstack_t *stack)
{
    program_list_init(&stack->data.programs);
    list_tstack_init(&stack->list);

    twidget_init(&stack->twidget);
    es_container_init(&stack->lines, sizeof(text_line_tstack_t));
    es_container_reserve(&stack->lines, 100);
    stack->twidget.interface = &proc_monitor_twidget_interface;
    stack->twidget.stack = stack;

    twidget_layout_init(&stack->layout);
    twidget_set_layout(&stack->twidget, &stack->layout);

    twidget_add_child(&stack->twidget, &stack->list.twidget);
}

char *_align_str(char *str, es_size_t n)
{
    const es_size_t len = strlen(str);
    if (len > n)
    {
        str += n;
        *str = '\0';
        return str;
    }
    str += len;
    for (int i = len; i < n; ++i)
    {
        *str++ = ' ';
    }
    *str = '\0';
    return str;
}

void print_time(char *dest, time_t time_sec)
{
    struct tm *time = localtime(&time_sec);
    char buf_h[100];
    char buf_m[100];
    char buf_s[100];
    buf_h[0] = buf_m[0] = buf_s[0] = '\0';
    const int hours = time_sec / 3600;
    const int minutes = (time_sec - 3600 * hours) / 60;
    const int seconds = (time_sec - 3600 * hours - 60 * minutes);
    if (hours > 0)
    {
        sprintf(buf_h, "%dh", hours);
    }
    if (minutes > 0)
    {
        if (minutes < 10)
        {
            sprintf(buf_m, "0%dm", minutes);
        }
        else
        {
            sprintf(buf_m, "%dm", minutes);
        }
    }
    if (seconds < 10)
    {
        sprintf(buf_s, "0%ds", seconds);
    }
    else
    {
        sprintf(buf_s, "%ds", seconds);
    }
    sprintf(dest, "%s%s%s", buf_h, buf_m, buf_s);
}

void _update_proc_monitor(twidget_t *twidget)
{
    es_size_t n_list_elements = twidget->size.y;
    proc_monitor_tstack_t *stack = twidget->stack;
    program_list_update(&stack->data.programs, n_list_elements);
    if (n_list_elements > stack->data.programs.programs.size)
    {
        n_list_elements = stack->data.programs.programs.size;
    }
    if (n_list_elements != 0)
    {
        --n_list_elements;
    }
    es_container_clear(&stack->lines);
    es_vector_clear(&stack->list.twidget.children);
    char buffer[1024];
    sprintf(
        buffer,
        "%s%s"
        "PROCESS NAME                  "
        "CPU (TOT)      "
        "CPU (AVG)   "
        "MEM (TOT)   "
        "MEM (AVG)   "
        "%s%s",
        get_background_color(CT_GREEN),
        get_format(CT_BOLD),
        get_background_color(CT_DEFAULT_COLOR),
        get_format(CT_DEFAULT_FORMAT));
    int alloc = es_container_push(&stack->lines, NULL);
    if (alloc)
    {
        return _update_proc_monitor(twidget);
    }
    text_line_tstack_t *line = es_container_get(&stack->lines, 0);
    text_line_tstack_init(line);
    text_line_set_content(line, buffer);
    twidget_add_child(&stack->list.twidget, &line->twidget);
    for (int i = 0; i != n_list_elements; ++i)
    {
        int alloc = es_container_push(&stack->lines, NULL);
        if (alloc)
        {
            return _update_proc_monitor(twidget);
        }
        text_line_tstack_t *line = es_container_get(&stack->lines, i + 1);
        text_line_tstack_init(line);
        char *dest = buffer;
        program_data_t *program = stack->data.programs.programs.data[i];
        sprintf(dest, "  > %s", es_string_get(&program->name));
        dest = _align_str(dest, 30);
        struct timespec diff;
        diff.tv_sec = program->time.tv_sec - program->time_ref.tv_sec;
        diff.tv_nsec = program->time.tv_nsec - program->time_ref.tv_nsec;
        unsigned long long diff_time_ms = diff.tv_sec * 1e3 + 1e-6 * diff.tv_nsec;
        if (diff_time_ms == 0)
        {
            sprintf(dest, "Initializing ...");
        }
        else
        {
            double cpu_avg = program->cpu_time * 100. / diff_time_ms;
            print_time(dest, program->cpu_time / 1000.);
            dest = _align_str(dest, 15);
            sprintf(dest, "%.2f %%", cpu_avg);
            // print_nice_memory(dest, program->memory);
            dest = _align_str(dest, 12);
            print_nice_memory(dest, program->memory);
            dest = _align_str(dest, 12);
            print_nice_memory(dest, (unsigned long long)(program->memory / (1e-3 * diff_time_ms)));
            _align_str(dest, 12);
        }
        text_line_set_content(line, buffer);
        twidget_add_child(&stack->list.twidget, &line->twidget);
    }
}

void _draw_proc_monitor(twidget_t *twidget)
{
}

void _free_proc_monitor(twidget_t *twidget)
{
    proc_monitor_tstack_t *stack = twidget->stack;
    program_list_free(&stack->data.programs);
    es_container_free(&stack->lines);
}
