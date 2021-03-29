#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_input(struct ast_node **result, struct sh_data *data)
{
    int rc;
    *result = NULL;
    if (lexer_consume(data->lexer, TOKEN_NEWLINE))
        return PARSER_DONE;
    if (lexer_consume(data->lexer, TOKEN_EOF))
        return PARSER_EOF;

    struct ast_node_input *input_node =
        ast_node_alloc_attach(NODE_INPUT, result);

    if ((rc = parse_list(&(input_node->list), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_input", data->lexer);

    if (lexer_consume(data->lexer, TOKEN_NEWLINE))
        return PARSER_DONE;
    if (lexer_consume(data->lexer, TOKEN_EOF))
        return PARSER_EOF;

    return syntax_error("parse_input", data->lexer);
}

void ast_free_input(struct ast_node *node)
{
    struct ast_node_input *input_node = to_node(node);
    ast_node_free(input_node->list);
}

void ast_print_input(FILE *file, struct ast_node *node)
{
    struct ast_node_input *input_node = to_node(node);

    ast_print_start_node(file, node, NULL);
    ast_print(file, input_node->list);
}
