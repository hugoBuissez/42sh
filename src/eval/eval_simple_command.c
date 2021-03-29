#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"
#include "ast_printer.h"
#include "builtins.h"
#include "eval.h"
#include "exec.h"
#include "expansion.h"
#include "list.h"
#include "redirections.h"
#include "variables.h"

static enum eval_status eval_simple_command_aux(struct argv_list *cmd_args,
                                                struct list *redirections,
                                                struct sh_data *data)
{
    enum eval_status exit_code;

    // Eval first argument if function call
    char *name = cmd_args->argv[0];
    struct ast_node_command *function_body;
    if ((function_body = hash_find(data->function_table, name)) != NULL)
    {
        exit_code = eval_node(to_node(function_body), cmd_args, data);
        return exit_code;
    }

    struct list *redirect_operations = apply_redirections(redirections);

    if (redirect_operations == NULL) // Failed to apply redirections
        return EVAL_ERROR;

    exit_code = exec_simple_command(cmd_args, data);

    restore_redirections(redirect_operations);

    return exit_code;
}

static void process_assignments(struct argv_list *assignments,
                                struct argv_list *args, struct sh_data *data)
{
    for (size_t i = 0; i < assignments->len; i++)
    {
        char *var_name = NULL;
        char *var_value = NULL;
        assignment_split(assignments->argv[i], &var_name, &var_value);

        // Expand variable value
        char *exp_value = expand_word_no_split(var_value, args, data);
        set_sh_variable(var_name, exp_value, false, data);

        free(var_name);
        free(var_value);
        free(exp_value);
    }
}

enum eval_status eval_simple_command(struct ast_node *node,
                                     struct argv_list *args,
                                     struct sh_data *data)
{
    struct ast_node_simple_command *command = to_node(node);
    enum eval_status exit_code = EVAL_TRUE;

    // Expand arguments
    struct argv_list *cmd_args =
        get_expanded_words(command->argument_list, args, data);

    process_assignments(command->assignments, args, data);

    if (cmd_args->len > 0)
        exit_code =
            eval_simple_command_aux(cmd_args, command->redirections, data);
    else
    {
        argv_list_free(cmd_args);
        return data->rc;
    }

    argv_list_free(cmd_args);

    return exit_code;
}
