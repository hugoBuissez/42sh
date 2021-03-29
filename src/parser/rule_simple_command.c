#include <stdlib.h>

#include "ast_printer.h"
#include "list.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_simple_command(struct ast_node **result,
                                        struct sh_data *data)
{
    int rc = 0;

    if (lexer_peek(data->lexer)->type != TOKEN_ASSIGNMENT_WORD
        && lexer_peek(data->lexer)->type != TOKEN_WORD)
        return PARSER_NO_MATCH;

    struct ast_node_simple_command *simple_command_node =
        ast_node_alloc_attach(NODE_SIMPLE_COMMAND, result);
    simple_command_node->argument_list = argv_list_init();
    simple_command_node->assignments = argv_list_init();
    simple_command_node->redirections = list_init();

    // Parse prefixes
    while ((rc = parse_prefix(simple_command_node->assignments,
                              simple_command_node->redirections, data))
           == PARSER_DONE)
        continue;
    if (rc == PARSER_ERROR)
        return rc;

    // Parse elements
    while ((rc = parse_element(simple_command_node->argument_list,
                               simple_command_node->redirections, data))
           == PARSER_DONE)
        continue;
    if (rc == PARSER_ERROR)
        return rc;

    return PARSER_DONE;
}

void ast_free_simple_command(struct ast_node *node)
{
    struct ast_node_simple_command *simple_command_node = to_node(node);

    argv_list_free(simple_command_node->argument_list);
    argv_list_free(simple_command_node->assignments);

    list_free_elements(simple_command_node->redirections, free_redirection);
    list_destroy(simple_command_node->redirections);
}

void ast_print_simple_command(FILE *file, struct ast_node *node)
{
    struct ast_node_simple_command *simple_command_node = to_node(node);

    // Print arguments
    fprintf(file, "\"");
    for (size_t i = 0; i < simple_command_node->argument_list->len; i++)
        fprintf(file, "%s ", simple_command_node->argument_list->argv[i]);

    // Print redirections
    struct list_iterator *iterator =
        iterator_init(simple_command_node->redirections);
    struct redirection *redirection = NULL;
    while ((redirection = iterator_next(iterator)))
        print_redirection(file, redirection);
    fprintf(file, "\";\n");
    free(iterator);
}
