#include <err.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_if(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_IF))
        return PARSER_NO_MATCH;

    struct ast_node_if *if_node = ast_node_alloc_attach(NODE_IF, result);

    if ((rc = parse_compound_list(&(if_node->condition), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_if", data->lexer);

    if (!lexer_consume(data->lexer, TOKEN_THEN))
        return syntax_error("parse_if", data->lexer);

    if ((rc = parse_compound_list(&(if_node->if_body), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_if", data->lexer);

    if ((rc = parse_else_clause(&(if_node->else_body), data)) == PARSER_ERROR)
        return PARSER_ERROR;

    if (!lexer_consume(data->lexer, TOKEN_FI))
        return syntax_error("parse_if", data->lexer);

    return PARSER_DONE;
}

void ast_free_if(struct ast_node *node)
{
    struct ast_node_if *if_node = to_node(node);
    ast_node_free(if_node->condition);
    ast_node_free(if_node->if_body);
    ast_node_free(if_node->else_body);
}

void ast_print_if(FILE *file, struct ast_node *node)
{
    struct ast_node_if *if_node = to_node(node);

    ast_print_end_node(file, node, NULL);

    ast_print_start_node(file, node, NULL);
    ast_print(file, if_node->condition);

    ast_print_start_node(file, node, NULL);
    ast_print(file, if_node->if_body);

    if (if_node->else_body == NULL)
        return;

    ast_print_start_node(file, node, NULL);
    ast_print(file, if_node->else_body);
}
