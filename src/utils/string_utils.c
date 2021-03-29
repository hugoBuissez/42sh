#include "string_utils.h"

#include <locale.h>
#include <stdlib.h>
#include <string.h>

char *str_n_replace(const char *str, const char *start, size_t n,
                    const char *new_w)
{
    size_t pre_len = start - str;
    size_t new_len = new_w ? strlen(new_w) : 0;

    size_t total_len = strlen(str);
    char *new_str = calloc(total_len - n + new_len + 1, sizeof(char));
    if (pre_len > 0)
        new_str = strncat(new_str, str, pre_len);
    if (new_w)
        new_str = strcat(new_str, new_w);
    new_str = strcat(new_str, str + pre_len + n); // Suffix

    return new_str;
}

char *str_replace(const char *str, const char *start, const char *old_w,
                  const char *new_w)
{
    size_t old_len = strlen(old_w);
    return str_n_replace(str, start, old_len, new_w);
}

void str_append(char **str, char *add)
{
    if (add == NULL)
        return;

    size_t str_len = *str ? strlen(*str) : 0;
    size_t total_size = str_len + strlen(add) + 1;
    *str = realloc(*str, total_size * sizeof(char));
    if (str_len == 0)
        **str = '\0';

    *str = strcat(*str, add);
}

void str_strip_trailing_newlines(char *str)
{
    if (!str)
        return;

    size_t len = strlen(str);
    for (size_t i = 0; i < len && str[len - 1 - i] == '\n'; ++i)
        str[len - 1 - i] = '\0';
}

int comp_str(const void *s1, const void *s2)
{
    char *const *f1 = s1;
    char *const *f2 = s2;

    setlocale(LC_COLLATE, "");
    return strcoll(*f1, *f2);
}
