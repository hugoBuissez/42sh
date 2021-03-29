#ifndef HASH_SET_H
#define HASH_SET_H

#include <stddef.h>

typedef size_t (*hash_function)(char *key, size_t table_size);

struct hash_slot
{
    char *name;
    struct hash_slot *next;
    void *data;
};

struct hash_set
{
    struct hash_slot **slots;
    size_t size;
    hash_function hf;
};

struct htab_iterator;

size_t basic_hash_function(char *key, size_t table_size);
struct hash_set *hash_init(size_t size, hash_function hf);
void hash_free(struct hash_set *set, void (*free_func)(void *));
int hash_insert(struct hash_set *set, char *name, void *data);
int hash_remove(struct hash_set *set, char *name, void (*free_func)(void *));
void *hash_find(const struct hash_set *set, char *name);
struct hash_slot *hash_find_slot(const struct hash_set *set, char *name);
void hash_dump(const struct hash_set *set);

struct htab_iterator *htab_iterator_init(struct hash_set *htab);
void *htab_iterator_get(struct htab_iterator *iterator);
void *htab_iterator_next(struct htab_iterator *iterator);
void htab_iterator_destroy(struct htab_iterator *iterator);

#endif /* !HASH_SET_H */
