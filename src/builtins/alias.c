#include "alias.h"

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "eval.h"
#include "htab.h"
#include "locale.h"
#include "string_utils.h"
#include "variables.h"

static int comp_alias(void *_s1, void *_s2)
{
    struct alias *alias1 = _s1;
    struct alias *alias2 = _s2;
    setlocale(LC_COLLATE, "");
    return strcoll(alias1->name, alias2->name);
}

static int free_return(char *name, char *value, int ret_val)
{
    free(name);
    free(value);
    return ret_val;
}

static void print_alias(bool print_header, struct alias *alias)
{
    if (!alias)
        return;

    printf("%s%s='%s'\n", print_header ? "alias " : "", alias->name,
           alias->raw_value);
}

static void print_aliasses(bool print_header, struct sh_data *data)
{
    if (!data)
        return;

    struct htab_iterator *htab_iter = htab_iterator_init(data->alias_table);
    struct alias *alias = NULL;

    // Get sorted aliases
    struct list *sorted_list = list_init();
    while ((alias = htab_iterator_next(htab_iter)))
        list_insert_sort(sorted_list, alias, comp_alias);

    // Print them
    struct list_iterator *list_iter = iterator_init(sorted_list);
    while ((alias = iterator_next(list_iter)))
        print_alias(print_header, alias);

    htab_iterator_destroy(htab_iter);
    iterator_destroy(list_iter);
    list_free_elements(sorted_list, NULL);
    list_destroy(sorted_list);
}

static int process_arg(char *arg, bool print_header, struct sh_data *data)
{
    char *name = NULL;
    char *value = NULL;
    assignment_split(arg, &name, &value);

    if (name == NULL)
        return free_return(name, value, EVAL_ERROR);

    if (value == NULL) // Print alias
    {
        struct alias *alias = hash_find(data->alias_table, name);
        if (alias == NULL)
        {
            warnx("alias: %s: not found", name);
            return free_return(name, value, EVAL_ERROR);
        }
        else
            print_alias(print_header, alias);
    }
    else // Add alias
    {
        add_alias(name, value, data->alias_table);
    }

    return free_return(name, value, EVAL_TRUE);
}

int builtin_alias(struct argv_list *args, struct sh_data *data)
{
    if (args->argv[1] == NULL)
    {
        print_aliasses(false, data);
        return EVAL_TRUE;
    }

    bool print_headers = false;

    size_t index = 1;
    if (strcmp(args->argv[index], "-p") == 0)
    {
        print_headers = true;
        print_aliasses(print_headers, data);
        index++;
    }

    int rc = EVAL_TRUE;
    while (index < args->len)
    {
        if (process_arg(args->argv[index], print_headers, data) != EVAL_TRUE)
            rc = EVAL_ERROR;
        index++;
    }

    return rc;
}

int builtin_unalias(struct argv_list *args, struct sh_data *data)
{
    char *alias_name = args->argv[1];

    if (!alias_name)
    {
        warnx("unalias: usage: unalias [-a] name [name ...]");
        return 2;
    }

    if (!hash_remove(data->alias_table, args->argv[1], free))
    {
        warnx("unalias: %s: not found", alias_name);
        return EVAL_ERROR;
    }

    return EVAL_TRUE;
}
