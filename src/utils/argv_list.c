#include "argv_list.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

struct argv_list *argv_list_init(void)
{
    struct argv_list *list = calloc(1, sizeof(*list));
    if (!list)
        errx(1, "Memory exhausted");

    list->argv = calloc(1, sizeof(char *));
    list->len = 0;

    return list;
}

char *argv_list_get_nth(struct argv_list *list, size_t nth)
{
    if (list == NULL)
        return NULL;

    if (nth >= list->len)
        return NULL;
    return list->argv[nth];
}

void argv_list_append(struct argv_list *list, char *element)
{
    if (list == NULL || element == NULL)
        return;

    list->argv = realloc(list->argv, (list->len + 2) * sizeof(char *));
    list->argv[list->len] = strdup(element);
    list->argv[list->len + 1] = NULL;
    list->len++;
}

void argv_list_free(struct argv_list *list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; list->argv[i] != NULL; i++)
        free(list->argv[i]);

    free(list->argv);
    free(list);
}
