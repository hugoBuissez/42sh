#include <string.h>

#include "argv_list.h"
#include "dynamic_string.h"
#include "expansion.h"
#include "string_utils.h"

void field_splitting(char *field, struct argv_list *words,
                     struct d_string *buffer, char *IFS)
{
    if (!field)
        return;

    for (char *ptr = field; ptr < (field + strlen(field)); ptr++)
    {
        char c = *ptr;

        if (in_list(c, IFS)) // Delim current word
        {
            if (string_len(buffer) > 0)
                argv_list_append(words, string_value(buffer));

            string_empty(buffer); // Clear buffer
        }
        else
            string_append_char(buffer, c);
    }
}
