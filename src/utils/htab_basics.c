#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

struct hash_set *hash_init(size_t size, hash_function hf)
{
    struct hash_set *new = malloc(sizeof(struct hash_set));
    if (!new)
        return NULL;

    new->slots = calloc(size, sizeof(struct hash_slot *));
    new->size = size;
    new->hf = hf;
    return new;
}

void hash_free(struct hash_set *set, void (*free_func)(void *))
{
    if (!set)
        return;

    for (size_t i = 0; i < set->size; i++)
    {
        struct hash_slot *cur = set->slots[i];
        while (cur)
        {
            struct hash_slot *tmp = cur;
            cur = cur->next;
            free(tmp->name);
            free_func(tmp->data);
            free(tmp);
        }
    }

    free(set->slots);
    free(set);
}

void hash_dump(const struct hash_set *set)
{
    for (size_t i = 0; i < set->size; i++)
    {
        struct hash_slot *current = set->slots[i];

        printf("[%zu] = ", i);
        while (current)
        {
            printf("%s", current->name);

            if (current->next)
                printf(" -> ");
            current = current->next;
        }
        printf("\n");
    }
}
