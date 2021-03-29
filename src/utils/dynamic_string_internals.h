#ifndef DYNAMIC_STRING_INTERNALS_H
#define DYNAMIC_STRING_INTERNALS_H

#include <stddef.h>

struct d_string
{
    size_t len;
    size_t buf_size;
    char *buf;
};

void expand_buffer(struct d_string *string, size_t expand_by);

#endif /* ! DYNAMIC_STRING_INTERNALS_H */
