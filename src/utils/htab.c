#include "htab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t basic_hash_function(char *key, size_t table_size)
{
    size_t hash = 0;
    size_t i = 0;
    for (; key[i] != '\0'; ++i)
        hash += key[i];
    hash += i;
    return hash % table_size;
}

static int is_in_list(struct hash_slot *slot, char *name)
{
    while (slot)
    {
        if (strcmp(slot->name, name) == 0)
            return 1;
        slot = slot->next;
    }
    return 0;
}

int hash_insert(struct hash_set *set, char *name, void *data)
{
    if (!set)
        return 0;

    size_t hash = set->hf(name, set->size);
    if (is_in_list(set->slots[hash], name))
        return 0;

    struct hash_slot *new = malloc(sizeof(struct hash_slot));
    new->name = name;
    new->data = data;

    struct hash_slot *temp = set->slots[hash];
    set->slots[hash] = new;
    new->next = temp;
    return 1;
}

int hash_remove(struct hash_set *set, char *name, void (*free_func)(void *))
{
    if (!set)
        return 0;

    size_t hash = set->hf(name, set->size);

    struct hash_slot *current = set->slots[hash];

    if (!current)
        return 0;

    if (strcmp(current->name, name) == 0)
    {
        set->slots[hash] = current->next;
        free_func(current->data);
        free(current->name);
        free(current);
        return 1;
    }

    while (current->next)
    {
        if (strcmp(current->next->name, name) == 0)
        {
            struct hash_slot *temp = current->next;
            current->next = current->next->next;
            free_func(temp->data);
            free(temp->name);
            free(temp);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

struct hash_slot *hash_find_slot(const struct hash_set *set, char *name)
{
    if (!set || !name)
        return NULL;
    size_t hash = set->hf(name, set->size);

    struct hash_slot *current = set->slots[hash];

    while (current)
    {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void *hash_find(const struct hash_set *set, char *name)
{
    if (!set || !name)
        return NULL;
    size_t hash = set->hf(name, set->size);

    struct hash_slot *current = set->slots[hash];

    while (current)
    {
        if (strcmp(current->name, name) == 0)
            return current->data;
        current = current->next;
    }
    return NULL;
}
