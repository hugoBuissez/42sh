#include <stdlib.h>

#include "list.h"
#include "list_internals.h"

bool list_empty(struct list *list)
{
    if (list == NULL)
        return true;
    return list->head == NULL;
}

size_t list_size(struct list *list)
{
    if (list == NULL)
        return 0;
    return list->size;
}

struct list *list_init(void)
{
    struct list *list = calloc(1, sizeof(*list));
    if (!list)
        return NULL;
    return list;
}

void list_free_elements(struct list *list, void (*free_func)(void *))
{
    if (list == NULL)
        return;

    void *data;
    while ((data = list_pop(list)))
    {
        if (free_func != NULL)
            free_func(data);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_destroy(struct list *list)
{
    free(list);
}
