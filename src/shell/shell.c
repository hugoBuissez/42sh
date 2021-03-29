#include "shell.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alias.h"
#include "ast.h"
#include "ast_printer.h"
#include "eval.h"
#include "htab.h"
#include "lexer.h"
#include "lexer_internals.h"
#include "parser.h"
#include "variables.h"

void setup_sub_shell(struct sh_data *data, struct sh_data *parent)
{
    if (parent)
    {
        data->function_table = parent->function_table;
        data->variable_table = parent->variable_table;
        data->alias_table = parent->alias_table;
    }

    data->current_input = NULL;
    data->lexer = lexer_init(data->options->stream);
    data->lexer->alias_table = data->alias_table;
    data->rc = 0;
    set_sh_variable("IFS", " \t\n", false, data);

    init_dot_file(data);
}

void setup_shell(struct sh_data *data)
{
    data->function_table = hash_init(20, basic_hash_function);
    data->variable_table = hash_init(20, basic_hash_function);
    data->alias_table = hash_init(20, basic_hash_function);
    setup_sub_shell(data, NULL);

    init_dot_file(data);
}

int run_shell(struct sh_data *data)
{
    enum eval_status eval_status = 0;
    struct ast_node *input = NULL;
    enum parser_status parser_status = PARSER_EOF;

    do
    {
        parser_status = parse_input(&input, data);

        if (parser_status != PARSER_DONE && parser_status != PARSER_EOF)
        {
            ast_node_free(input);
            data->current_input = NULL;
            return 2;
        }

        if (input != NULL)
        {
            add_input_to_dot_file(data, input);

            data->current_input = input;
            eval_status = eval_node(input, data->options->args, data);
            ast_node_free(input);
            data->current_input = NULL;
        }
    } while (parser_status != PARSER_EOF);

    return eval_status;
}

void end_sub_shell(struct sh_data *data)
{
    ast_node_free(data->current_input);
    lexer_destroy(data->lexer);
    free_options(data->options);
}

void end_shell(struct sh_data *data)
{
    end_dot_file(data);

    hash_free(data->function_table, ast_node_free);
    hash_free(data->variable_table, free_sh_variable);
    hash_free(data->alias_table, free_alias);
    end_sub_shell(data);
}
