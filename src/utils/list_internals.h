#ifndef LIST_INTERNALS_H
#define LIST_INTERNALS_H

#include <stddef.h>

struct list_element
{
    void *data;
    struct list_element *next;
};

struct list
{
    struct list_element *head;
    struct list_element *tail;
    size_t size;
};

#endif /* ! LIST_INTERNALS_H */
