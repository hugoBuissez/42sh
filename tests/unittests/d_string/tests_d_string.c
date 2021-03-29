#include <criterion/criterion.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dynamic_string.h"
#include "dynamic_string_internals.h"

Test(d_string, create_destroy)
{
    struct d_string *string = string_create("");
    string_destroy(string);
}

Test(d_string, string_append)
{
    struct d_string *string = string_create("");
    cr_expect(string->len == 0);
    cr_expect(string->buf_size == 1, "Got %lu", string->buf_size);

    string_append_char(string, 'H');
    string_append_char(string, 'e');
    string_append_char(string, 'l');
    string_append_char(string, 'l');
    string_append_char(string, 'o');
    cr_expect_str_eq(string->buf, "Hello", "Got %s", string->buf);

    cr_expect(string->len == 5);
    cr_expect(string->buf_size == 8, "Got %lu", string->buf_size);

    string_destroy(string);
}
