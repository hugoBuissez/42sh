#include <stdio.h>
#include <stdlib.h>

#include "string_utils.h"

bool in_range(char c, const char start, const char end)
{
    return c >= start && c <= end;
}

bool in_list(char c, const char *list)
{
    for (size_t i = 0; list[i]; i++)
        if (c == list[i])
            return true;
    return false;
}

bool match_seq(char *input, char *seq)
{
    if (!input || !seq)
        return false;

    size_t i = 0;
    for (; input[i] && seq[i] && input[i] == seq[i]; i++)
        continue;

    return seq[i] == '\0';
}

char *itao(int n)
{
    char *res = calloc(32, sizeof(char));
    sprintf(res, "%d", n);
    return res;
}
