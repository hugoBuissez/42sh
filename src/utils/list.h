#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

struct list;
struct list_iterator;

// List management
bool list_empty(struct list *list);
size_t list_size(struct list *list);
struct list *list_init(void);
void list_free_elements(struct list *list, void (*free_func)(void *));
void list_destroy(struct list *list);

// List usage
void list_push(struct list *list, void *data);
void list_append(struct list *list, void *data);
void list_insert_sort(struct list *list, void *data,
                      int (*comp)(void *, void *));
void *list_pop(struct list *list);
void *list_peek(struct list *list);

// List iterator
struct list_iterator *iterator_init(struct list *list);
void *iterator_get(struct list_iterator *iterator);
void *iterator_next(struct list_iterator *iterator);
void iterator_destroy(struct list_iterator *iterator);

#endif /* ! LIST_H */
