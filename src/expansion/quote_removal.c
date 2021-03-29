#include <stdlib.h>

#include "dynamic_string.h"
#include "expansion.h"
#include "string.h"
#include "string_utils.h"

static bool is_esc(char *ptr, char *last_escaped)
{
    return ptr == last_escaped;
}

static void in_quote_removal(char *str, char **_ptr, char quote,
                             struct d_string *buffer)
{
    char *ptr = *_ptr + 1;
    char *last_escaped = ptr - 1;

    for (; ptr < (str + strlen(str)); ptr++)
    {
        char c = *ptr;

        if (c == '\\' && quote == '\"' && in_list(*(ptr + 1), "$`\"\\\n"))
        {
            last_escaped = ptr + 1;
            continue;
        }

        if (c == quote && !is_esc(ptr, last_escaped))
            break;
        else
            string_append_char(buffer, c);
    }

    *_ptr = ptr;
}

void quote_removal(char **word)
{
    struct d_string *buffer = string_create("");

    char *str = *word;
    char *last_escaped = str - 1;

    for (char *ptr = str; ptr < (str + strlen(str)); ptr++)
    {
        char c = *ptr;

        if (c == '\\' && !is_esc(ptr, last_escaped)) // Escape character
        {
            last_escaped = ptr + 1;
            continue;
        }

        if ((c == '\'' || c == '\"') && !is_esc(ptr, last_escaped))
            in_quote_removal(str, &ptr, c, buffer);
        else
            string_append_char(buffer, c);
    }

    free(*word);
    *word = string_value_copy(buffer);

    string_destroy(buffer);
}
