#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>

#include "shell.h"

struct sh_variable
{
    bool exported;
    char *name;
    char *value;
};

void set_sh_variable(char *name, char *value, bool export,
                     struct sh_data *data);
char *find_sh_variable(char *name, struct sh_data *data);

void unexport_sh_variable(char *name, struct sh_data *data);
void free_sh_variable(void *sh_var);

void assignment_split(char *assignment, char **name, char **value);

#endif /* ! VARIABLES_H */
