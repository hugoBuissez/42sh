#include "variables.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

static void s_setenv(char *name, char *value)
{
    if (name)
        setenv(name, value ? value : "", 1);
}

static void add_new_sh_variable(char *name, char *value, bool export,
                                struct sh_data *data)
{
    if (value == NULL)
        value = "";

    struct sh_variable *sh_var = calloc(1, sizeof(*sh_var));
    if (sh_var == NULL)
        errx(1, "Memory exhausted");

    // Check if environment variable
    if (export || getenv(name) != NULL)
    {
        sh_var->exported = true;
        sh_var->name = strdup(name);
        s_setenv(name, value);
    }
    else // Else add new shell variable
    {
        sh_var->exported = export;
        if (export)
            s_setenv(name, value);
        sh_var->name = strdup(name);
        sh_var->value = strdup(value);
    }

    hash_insert(data->variable_table, strdup(name), sh_var);
}

void set_sh_variable(char *name, char *value, bool export, struct sh_data *data)
{
    struct sh_variable *var = hash_find(data->variable_table, name);
    if (var == NULL)
    {
        // If variable does not exist create new entry
        add_new_sh_variable(name, value, export, data);
        return;
    }

    // If set to export and no given value and no given value, do nothing
    if (export && var->exported && value == NULL)
        return;

    // If set to export and no given value, export current variable as is
    if (export && value == NULL)
    {
        s_setenv(name, var->value);
        var->exported = true;
        free(var->value);
        var->value = NULL;
        return;
    }

    // Else update variable
    free(var->value);

    if (var->exported)
        s_setenv(name, value);
    else
        var->value = strdup(value);
}

char *find_sh_variable(char *name, struct sh_data *data)
{
    struct sh_variable *var = hash_find(data->variable_table, name);

    if (var != NULL && !var->exported)
        return var->value;

    return getenv(name);
}

void unexport_sh_variable(char *name, struct sh_data *data)
{
    struct sh_variable *var = hash_find(data->variable_table, name);

    if (var != NULL)
    {
        var->exported = false;
        free(var->value);
        var->value = strdup(getenv(name));
    }
    unsetenv(name);
}

void free_sh_variable(void *_sh_var)
{
    struct sh_variable *sh_var = _sh_var;

    unsetenv(sh_var->name);

    free(sh_var->name);
    free(sh_var->value);
    free(sh_var);
}
