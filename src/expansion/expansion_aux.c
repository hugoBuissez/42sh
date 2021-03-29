#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"
#include "dynamic_string.h"
#include "expansion.h"
#include "string_utils.h"
#include "variables.h"

char *expand_tilde(struct expansion_data *exp_data)
{
    char *word = exp_data->input;

    if (*word != '~')
        return NULL;

    char *home = find_sh_variable("HOME", exp_data->data);
    char *pwd = find_sh_variable("PWD", exp_data->data);
    char *old_pwd = find_sh_variable("OLDPWD", exp_data->data);

    exp_data->exp_len = 1; // Indicate how long the og expansion is
    if (word[1] == '\0')
        return strdup(home);
    if (word[1] == '/')
        return strdup(home);

    exp_data->exp_len = 2; // Indicate how long the og expansion is
    if (word[1] == '+' && (word[2] == '/' || word[2] == '\0'))
        return strdup(pwd);
    if (word[1] == '-' && (word[2] == '/' || word[2] == '\0') && old_pwd)
        return strdup(old_pwd);

    return NULL;
}

char *expand_command(struct expansion_data *exp_data)
{
    // $(...)

    if (exp_data->exp_len <= 3)
        return NULL;

    char *input = strndup(exp_data->input + 2, exp_data->exp_len - 3);
    char *cmd_ouput = eval_command_sub(input, exp_data->data);
    free(input);

    // Remove trailing newlines
    str_strip_trailing_newlines(cmd_ouput);

    return cmd_ouput;
}

char *expand_backtick_command(struct expansion_data *exp_data)
{
    // `...`

    if (exp_data->exp_len <= 2)
        return NULL;

    char *input = strndup(exp_data->input + 1, exp_data->exp_len - 2);
    char *cmd_ouput = eval_command_sub(input, exp_data->data);
    free(input);

    // Remove trailing newlines
    str_strip_trailing_newlines(cmd_ouput);

    return cmd_ouput;
}

char *expand_arithmetic(struct expansion_data *exp_data)
{
    // $((...))

    if (exp_data->exp_len <= 5)
        return NULL;

    char *input = strndup(exp_data->input + 3, exp_data->exp_len - 5);
    char *exp_input =
        expand_word_no_split(input, exp_data->args, exp_data->data);

    char *output = eval_arithmetic_expr(exp_input);

    free(exp_input);
    free(input);

    return output;
}

char *expand(struct expansion_data *exp_data)
{
    if (match_seq(exp_data->input, "$(("))
        return expand_arithmetic(exp_data);
    if (match_seq(exp_data->input, "$("))
        return expand_command(exp_data);
    if (match_seq(exp_data->input, "`"))
        return expand_backtick_command(exp_data);
    if (match_seq(exp_data->input, "${"))
        return expand_bracket_param(exp_data);
    if (match_seq(exp_data->input, "$"))
        return expand_param(exp_data);

    return NULL;
}
