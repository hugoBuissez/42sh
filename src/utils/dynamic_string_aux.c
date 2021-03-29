#include <string.h>

#include "dynamic_string.h"
#include "dynamic_string_internals.h"

void string_append_char(struct d_string *string, char c)
{
    if (!string)
        return;

    expand_buffer(string, 1);
    string->buf[string->len] = c;
    string->buf[string->len + 1] = '\0';
    string->len++;
}

void string_pop_last_char(struct d_string *string)
{
    if (!string)
        return;

    string->buf[string->len - 1] = '\0';
    string->len--;
}

void string_empty(struct d_string *string)
{
    if (!string)
        return;

    string->buf[0] = '\0';
    string->len = 0;
}

void string_truncate_at(struct d_string *string, size_t n)
{
    if (string == NULL || n >= string->len)
        return;
    string->buf[n] = 0;
    string->len = strlen(string->buf);
}
