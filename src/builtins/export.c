#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "builtins.h"
#include "string_utils.h"
#include "variables.h"

extern char **environ;

static void export_print(void)
{
    size_t len = 0;
    for (char **var = environ; *var != NULL; var++)
        len++;

    qsort(environ, len, sizeof(char *), comp_str);

    for (char **var = environ; *var != NULL; var++)
    {
        char *name = NULL;
        char *value = NULL;
        assignment_split(*var, &name, &value);

        if (value && strcmp(value, "") != 0)
            printf("export %s=\"%s\"\n", name, value);
        else
            printf("export %s\n", name);

        fflush(stdout);

        free(name);
        free(value);
    }
}

struct export_options
{
    bool opt_n;
    bool opt_p;
    bool opt_f;
};

int get_export_options(int argc, char *argv[], size_t *index,
                       struct export_options *options)
{
    options->opt_n = false;
    options->opt_p = false;
    options->opt_f = false;

    int opt;
    while ((opt = getopt(argc, argv, "npf")) != -1)
    {
        switch (opt)
        {
        case 'n':
            options->opt_n = true;
            break;
        case 'p':
            options->opt_p = true;
            break;
        case 'f':
            options->opt_f = true;
            break;
        default:
            warnx("export: usage: export [-fn] [name[=value] ...] or export "
                  "-p");
            return 2;
        }
    }
    *index = optind;

    return 0;
}

static int process_arg(char *arg, struct export_options *options,
                       struct sh_data *data)
{
    char *name = NULL;
    char *value = NULL;
    assignment_split(arg, &name, &value);

    if (options->opt_f)
    {
        struct ast_node *function = hash_find(data->function_table, name);
        if (function == NULL)
        {
            warnx("export: toto: not a function");
            free(name);
            free(value);
            return 1;
        }

        if (options->opt_n)
            unexport_sh_variable(name, data);
        else
            set_sh_variable(name, NULL, true, data);
    }
    else
    {
        if (options->opt_n)
            unexport_sh_variable(name, data);
        else
            set_sh_variable(name, value, true, data);
    }

    free(name);
    free(value);

    return 0;
}

int builtin_export(struct argv_list *args, struct sh_data *data)
{
    if (args->len == 1)
    {
        export_print();
        return 0;
    }

    size_t index = 0;
    struct export_options options;
    int rc = get_export_options(args->len, args->argv, &index, &options);
    if (rc != 0)
        return rc;

    if (options.opt_p && index == args->len)
    {
        export_print();
        return 0;
    }

    while (index < args->len)
    {
        rc = process_arg(args->argv[index++], &options, data);
        if (rc != 0)
            return rc;
    }

    return 0;
}
