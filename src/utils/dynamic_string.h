#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stddef.h>

struct d_string;

struct d_string *string_create(const char *str);
void string_append(struct d_string *string, char *new);
void string_append_n(struct d_string *string, char *src, size_t n);
void string_destroy(struct d_string *string);

size_t string_len(struct d_string *string);
char *string_value(struct d_string *string);
char *string_value_copy(struct d_string *string);

void string_append_char(struct d_string *string, char c);
void string_pop_last_char(struct d_string *string);
void string_empty(struct d_string *string);
void string_truncate_at(struct d_string *string, size_t n);

#endif /* ! DYNAMIC_STRING_H */
