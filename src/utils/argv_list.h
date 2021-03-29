#ifndef ARGV_LIST_H
#define ARGV_LIST_H

#include <stddef.h>

struct argv_list
{
    char **argv;
    size_t len;
};

struct argv_list *argv_list_init(void);
char *argv_list_get_nth(struct argv_list *list, size_t nth);
void argv_list_append(struct argv_list *list, char *element);
void argv_list_free(struct argv_list *list);

struct argv_list *argv_list_clone(struct argv_list *list);
void argv_list_concat(struct argv_list *list1, struct argv_list *list2);

// Argv list

#endif /* ! ARGV_LIST_H */
