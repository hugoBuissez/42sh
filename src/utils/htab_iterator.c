#include <stdio.h>
#include <stdlib.h>

#include "htab.h"

struct htab_iterator
{
    struct hash_slot *element;
    size_t index;
    struct hash_set *htab;
};

struct htab_iterator *htab_iterator_init(struct hash_set *htab)
{
    struct htab_iterator *iterator = calloc(1, sizeof(*iterator));
    if (!iterator)
        return NULL;

    iterator->element = htab->slots[0];
    iterator->index = 0;
    iterator->htab = htab;
    return iterator;
}

void *htab_iterator_get(struct htab_iterator *iterator)
{
    if (iterator == NULL || iterator->element == NULL)
        return NULL;

    return iterator->element->data;
}

void *htab_iterator_next(struct htab_iterator *iterator)
{
    if (iterator == NULL)
        return NULL;

    while (iterator->element == NULL
           && iterator->index < iterator->htab->size - 1)
    {
        iterator->index++;
        iterator->element = iterator->htab->slots[iterator->index];
    }

    if (iterator->element == NULL)
        return NULL;

    void *data = iterator->element->data;
    iterator->element = iterator->element->next;

    return data;
}

void htab_iterator_destroy(struct htab_iterator *iterator)
{
    free(iterator);
}
