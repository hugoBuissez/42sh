#include <string.h>

#include "dynamic_string.h"
#include "dynamic_string_internals.h"

size_t string_len(struct d_string *string)
{
    return string->len;
}

char *string_value(struct d_string *string)
{
    return string->buf;
}

char *string_value_copy(struct d_string *string)
{
    return strdup(string->buf);
}
