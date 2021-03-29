#include <stdlib.h>
#include <string.h>

#include "variables.h"

void assignment_split(char *assignment, char **name, char **value)
{
    if (!assignment)
        return;

    char *eq_ptr = strchr(assignment, '=');

    if (eq_ptr == NULL)
    {
        *name = strdup(assignment);
        *value = NULL;
    }
    else
    {
        *name = strndup(assignment, eq_ptr - assignment);
        *value = strdup(eq_ptr + 1);
    }
}
