#include "proc_monitor.h"
#include "eco_perf/cute_terminal/io/string_utils.h"
#include "eco_perf/cute_terminal/widgets/text_line.h"
#include <stdio.h>
#include <string.h>

void _update_proc_monitor(twidget_t *twidget);
void _draw_proc_monitor(twidget_t *twidget);
void _free_proc_monitor(twidget_t *twidget);

const twidget_interface_t proc_monitor_twidget_interface = {
    _update_proc_monitor,
    _draw_proc_monitor,
    _free_proc_monitor};

void proc_monitor_tstack_init(proc_monitor_tstack_t *stack)
{
    process_list_init(&stack->data.process_list);
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

bool _sort_cpu_decreasing(es_cref_t p1_ref, es_cref_t p2_ref)
{
    process_data_t const *p1 = p1_ref;
    process_data_t const *p2 = p2_ref;
    double p1_cpu = p1->cpu_usage.nice_time + p1->cpu_usage.sys_time + p1->cpu_usage.user_time;
    double p2_cpu = p2->cpu_usage.nice_time + p2->cpu_usage.sys_time + p2->cpu_usage.user_time;
    return p1_cpu > p2_cpu;
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

void _update_proc_monitor(twidget_t *twidget)
{
    es_size_t n_list_elements = twidget->size.y;
    proc_monitor_tstack_t *stack = twidget->stack;
    process_list_update(&stack->data.process_list);
    if (n_list_elements > stack->data.process_list.processes.size)
    {
        n_list_elements = stack->data.process_list.processes.size;
    }
    process_list_sort_view(&stack->data.process_list, _sort_cpu_decreasing);
    es_container_clear(&stack->lines);
    es_vector_clear(&stack->list.twidget.children);
    for (int i = 0; i != n_list_elements; ++i)
    {
        int alloc = es_container_push(&stack->lines, NULL);
        if (alloc)
        {
            return _update_proc_monitor(twidget);
        }
        text_line_tstack_t *line = es_container_get(&stack->lines, i);
        text_line_tstack_init(line);
        char buffer[1024];
        char *dest = buffer;
        process_data_t *process = stack->data.process_list.processes.data[i];
        sprintf(dest, "  > %s", es_string_get(&process->executable));
        dest = _align_str(dest, 30);
        sprintf(
            dest,
            "cpu: %.2fs",
            process->cpu_usage.nice_time + process->cpu_usage.user_time + process->cpu_usage.sys_time);
        dest = _align_str(dest, 20);
        char const real_mem_name[] = "mem(R) ";
        char const virt_mem_name[] = "mem(V) ";
        char const shared_mem_name[] = "mem(S) ";
        strcpy(dest, real_mem_name);
        print_nice_memory(dest + sizeof(real_mem_name) - 1, process->memory_usage.real);
        dest = _align_str(dest, 20);
        strcpy(dest, virt_mem_name);
        print_nice_memory(dest + sizeof(virt_mem_name) - 1, process->memory_usage.virt);
        dest = _align_str(dest, 20);
        strcpy(dest, shared_mem_name);
        print_nice_memory(dest + sizeof(shared_mem_name) - 1, process->memory_usage.shared);
        _align_str(dest, 20);
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
    process_list_free(&stack->data.process_list);
    es_container_free(&stack->lines);
}
