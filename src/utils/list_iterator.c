#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_internals.h"

struct list_iterator
{
    struct list_element *element;
};

struct list_iterator *iterator_init(struct list *list)
{
    struct list_iterator *iterator = calloc(1, sizeof(*iterator));

    if (!iterator)
        return NULL;

    iterator->element = list->head;

    return iterator;
}

void *iterator_get(struct list_iterator *iterator)
{
    if (iterator == NULL || iterator->element == NULL)
        return NULL;

    return iterator->element->data;
}

void *iterator_next(struct list_iterator *iterator)
{
    if (iterator == NULL || iterator->element == NULL)
        return NULL;

    void *data = iterator->element->data;
    iterator->element = iterator->element->next;

    return data;
}

void iterator_destroy(struct list_iterator *iterator)
{
    free(iterator);
}
