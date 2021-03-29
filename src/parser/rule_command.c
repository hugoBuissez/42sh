#include <stdlib.h>

#include "ast_printer.h"
#include "htab.h"
#include "parser.h"
#include "token.h"

static enum parser_status parse_command_aux(struct ast_node **result,
                                            struct sh_data *data,
                                            struct ast_node *command,
                                            char *func_name)
{
    struct ast_node_command *command_node =
        ast_node_alloc_attach(NODE_COMMAND, result);
    command_node->redirections = list_init();
    command_node->command = command;

    // Parse redirections
    int rc;
    struct redirection *redirection = NULL;
    while ((rc = parse_redirection(&redirection, data)) == PARSER_DONE)
        list_append(command_node->redirections, redirection);

    if (rc == PARSER_ERROR)
    {
        ast_node_free(command_node);
        return PARSER_ERROR;
    }

    if (func_name != NULL)
    {
        if (hash_find(data->function_table, func_name))
            hash_remove(data->function_table, func_name, ast_node_free);
        hash_insert(data->function_table, func_name, command_node);
        *result = NULL;
    }

    return PARSER_DONE;
}

enum parser_status parse_command(struct ast_node **result, struct sh_data *data)
{
    int rc = PARSER_NO_MATCH;
    struct ast_node *command = NULL;
    bool is_funcdec = lexer_is_funcdec(data->lexer);

    if (!is_funcdec
        && (rc = parse_simple_command(&(command), data)) == PARSER_ERROR)
    {
        ast_node_free(command);
        return PARSER_ERROR;
    }

    char *func_name = NULL;
    if (rc == PARSER_NO_MATCH
        && (rc = parse_funcdec(&func_name, &(command), data)) == PARSER_ERROR)
    {
        free(func_name);
        ast_node_free(command);
        return PARSER_ERROR;
    }

    if (rc == PARSER_NO_MATCH
        && (rc = parse_shell_command(&(command), data)) != PARSER_DONE)
    {
        ast_node_free(command);
        return rc;
    }

    return parse_command_aux(result, data, command, func_name);
}

void ast_free_command(struct ast_node *node)
{
    struct ast_node_command *command_node = to_node(node);
    ast_node_free(command_node->command);
    list_free_elements(command_node->redirections, free_redirection);
    list_destroy(command_node->redirections);
}

void ast_print_command(FILE *file, struct ast_node *node)
{
    struct ast_node_command *command_node = to_node(node);
    ast_print_end_node(file, node, NULL);

    ast_print_start_node(file, node, NULL);
    ast_print(file, command_node->command);
}
