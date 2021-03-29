#include <err.h>
#include <stdlib.h>

#include "list.h"
#include "list_internals.h"

static struct list_element *create_element(void *data)
{
    struct list_element *element = calloc(1, sizeof(*element));
    if (!element)
        errx(1, "Memory exhausted");
    element->data = data;
    return element;
}

void list_push(struct list *list, void *data)
{
    if (!list)
        return;
    struct list_element *element = create_element(data);

    if (list_empty(list))
    {
        list->head = element;
        list->tail = element;
        list->size++;
        return;
    }

    element->next = list->head;
    list->head = element;
    list->size++;
}

void list_append(struct list *list, void *data)
{
    if (!list)
        return;
    struct list_element *element = create_element(data);

    if (list_empty(list)) // Empty list
    {
        list->head = element;
        list->tail = element;
        list->size++;
        return;
    }

    list->tail->next = element;
    list->tail = element;
    list->size++;
}

void list_insert_sort(struct list *list, void *data,
                      int (*comp)(void *, void *))
{
    if (!list)
        return;

    if (list_empty(list) || comp(data, list->head->data) < 0)
    {
        list_push(list, data);
        return;
    }

    struct list_element *new = create_element(data);

    struct list_element *current = list->head;
    // Search pos
    while (current && current->next && comp(current->next->data, data) < 0)
        current = current->next;

    if (current == list->tail)
    {
        list_append(list, data);
        free(new);
        return;
    }

    // Else insert
    new->next = current->next;
    current->next = new;
    list->size++;
}

void *list_pop(struct list *list)
{
    if (!list || list_empty(list))
        return NULL;

    struct list_element *element = list->head;

    list->head = element->next;
    list->size--;

    void *data = element->data;
    free(element);

    return data;
}

void *list_peek(struct list *list)
{
    if (!list || list_empty(list))
        return NULL;

    struct list_element *element = list->head;

    return element->data;
}
