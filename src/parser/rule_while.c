#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_while(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_WHILE))
        return PARSER_NO_MATCH;

    struct ast_node_while *while_node =
        ast_node_alloc_attach(NODE_WHILE, result);

    if ((rc = parse_compound_list(&(while_node->condition), data))
        != PARSER_DONE)
        return not_done_error(rc, "parse_while", data->lexer);

    if ((rc = parse_do_group(&(while_node->while_body), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_while", data->lexer);

    return PARSER_DONE;
}

void ast_free_while(struct ast_node *node)
{
    struct ast_node_while *while_node = to_node(node);
    ast_node_free(while_node->while_body);
    ast_node_free(while_node->condition);
}

void ast_print_while(FILE *file, struct ast_node *node)
{
    struct ast_node_while *while_node = to_node(node);

    ast_print_end_node(file, node, NULL);

    ast_print_start_node(file, node, NULL);
    ast_print(file, while_node->condition);

    ast_print_start_node(file, node, NULL);
    ast_print(file, while_node->while_body);
}
