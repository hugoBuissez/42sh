#ifndef ALIAS_H
#define ALIAS_H

#include "list.h"
#include "shell.h"

struct alias
{
    char *name;
    char *raw_value;
    struct list *tokens;
};

void add_alias(char *name, char *value, struct hash_set *alias_table);
void free_alias(void *_alias);

#endif /* ! ALIAS_H */
