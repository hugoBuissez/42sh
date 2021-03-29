#include "argv_list.h"

struct argv_list *argv_list_clone(struct argv_list *list)
{
    if (list == NULL)
        return NULL;

    struct argv_list *new = argv_list_init();
    for (size_t i = 0; i < list->len; i++)
        argv_list_append(new, list->argv[i]);

    return new;
}

void argv_list_concat(struct argv_list *list1, struct argv_list *list2)
{
    if (list1 == NULL || list2 == NULL)
        return;

    for (size_t i = 0; i < list2->len; i++)
        argv_list_append(list1, list2->argv[i]);

    argv_list_free(list2);
}
