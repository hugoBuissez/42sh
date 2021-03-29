#include "dynamic_string.h"

#include <stdlib.h>
#include <string.h>

#include "dynamic_string_internals.h"

static size_t nearest_pow_2(size_t n)
{
    n--;
    n = n | n >> 1 | n >> 2 | n >> 4 | n >> 8 | n >> 16 | n >> 32;
    n++;
    return n;
}

struct d_string *string_create(const char *str)
{
    struct d_string *string = malloc(sizeof(*string));
    if (!string)
        return NULL;

    string->len = strlen(str);
    string->buf_size = nearest_pow_2(string->len + 1);
    string->buf = malloc(string->buf_size * sizeof(char));
    strcpy(string->buf, str);

    return string;
}

void expand_buffer(struct d_string *string, size_t expand_by)
{
    if (!string)
        return;

    size_t new_len = string->len + expand_by + 1;
    if (new_len >= string->buf_size)
    {
        size_t new_buf_size = nearest_pow_2(new_len);
        string->buf = realloc(string->buf, new_buf_size * sizeof(char));
        string->buf_size = new_buf_size;
    }
}

void string_append(struct d_string *string, char *new)
{
    if (!string || !new)
        return;

    size_t new_len = strlen(new);
    expand_buffer(string, new_len);
    strcat(string->buf, new);
    string->len += new_len;
}

void string_append_n(struct d_string *string, char *src, size_t n)
{
    if (!string || !src)
        return;

    expand_buffer(string, n);
    strncat(string->buf, src, n);
    string->len += n;
}

void string_destroy(struct d_string *string)
{
    if (!string)
        return;

    free(string->buf);
    free(string);
}
