#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "argv_list.h"
#include "dynamic_string.h"
#include "expansion.h"
#include "lexer_internals.h"
#include "string_utils.h"
#include "variables.h"

static char *concat_args(struct argv_list *args, struct sh_data *data)
{
    struct d_string *buf = string_create("");

    char *IFS = find_sh_variable("IFS", data);

    for (size_t i = 1; i < args->len; i++)
    {
        string_append(buf, argv_list_get_nth(args, i));
        if (IFS[0] && i < args->len - 1)
            string_append_char(buf, IFS[0]); // First char of IFS
    }

    char *res = string_value_copy(buf);
    string_destroy(buf);

    return res;
}

static char *get_rand(void)
{
    char *res = calloc(16, sizeof(char));
    int int_rand = rand() % 32767;
    sprintf(res, "%d", int_rand);
    return res;
}

static char *expand_special_param(struct expansion_data *exp_data)
{
    char c = *(exp_data->input + 1);

    if (!isdigit(c) && !in_list(c, "*@?#$"))
        return NULL;

    exp_data->exp_len = 2; // Mark how long the expansion is ($ included)

    if (c == '@')
        exp_data->force_split = true;

    if (isdigit(c))
    {
        char *arg = argv_list_get_nth(exp_data->args, c - '0');
        return strdup(arg ? arg : "");
    }
    else if (c == '*' || c == '@')
        return concat_args(exp_data->args, exp_data->data);
    else if (c == '?')
        return itao(exp_data->data->rc);
    else if (c == '#')
        return itao(exp_data->args->len - 1);
    else if (c == '$')
        return itao(getpid());

    return NULL;
}

static size_t longest_valid_name(char *word)
{
    if (!valid_name_start(word[0]))
        return 0;

    size_t i = 1;
    for (; word[i] && valid_name_char(word[i]); i++)
        continue;
    return i;
}

static char *expand_simple_param(struct expansion_data *exp_data)
{
    size_t len = longest_valid_name(exp_data->input + 1);
    if (len == 0)
        return 0;

    exp_data->exp_len = len + 1; // Mark how long the expansion is ($ included)

    char *res = NULL;
    char *var_name = strndup(exp_data->input + 1, len);
    char *var_value = NULL;

    if (strcmp(var_name, "RANDOM") == 0)
        res = get_rand();
    else if (strcmp(var_name, "UID") == 0)
        res = itao(getpid());
    else if ((var_value = find_sh_variable(var_name, exp_data->data)))
        res = strdup(var_value);

    free(var_name);

    return res;
}

char *expand_bracket_param(struct expansion_data *exp_data)
{
    size_t len = longest_valid_name(exp_data->input + 2);
    if (len == 0)
        return 0;

    char *res = NULL;
    char *var_name = strndup(exp_data->input + 2, len);
    char *var_value = NULL;

    if (strcmp(var_name, "RANDOM") == 0)
        res = get_rand();
    else if (strcmp(var_name, "UID") == 0)
        res = itao(getpid());
    else if ((var_value = find_sh_variable(var_name, exp_data->data)))
        res = strdup(var_value);

    free(var_name);

    return res;
}

char *expand_param(struct expansion_data *exp_data)
{
    char *res = NULL;

    if ((res = expand_special_param(exp_data)))
        return res;
    if ((res = expand_simple_param(exp_data)))
        return res;

    return res;
}