#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_until(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_UNTIL))
        return PARSER_NO_MATCH;

    struct ast_node_until *until_node =
        ast_node_alloc_attach(NODE_UNTIL, result);

    if ((rc = parse_compound_list(&(until_node->condition), data))
        != PARSER_DONE)
        return not_done_error(rc, "parse_until", data->lexer);

    if ((rc = parse_do_group(&(until_node->until_body), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_until", data->lexer);

    return PARSER_DONE;
}

void ast_free_until(struct ast_node *node)
{
    struct ast_node_until *until_node = to_node(node);
    ast_node_free(until_node->until_body);
    ast_node_free(until_node->condition);
}

void ast_print_until(FILE *file, struct ast_node *node)
{
    struct ast_node_until *until_node = to_node(node);

    ast_print_end_node(file, node, NULL);

    ast_print_start_node(file, node, NULL);
    ast_print(file, until_node->condition);

    ast_print_start_node(file, node, NULL);
    ast_print(file, until_node->until_body);
}
