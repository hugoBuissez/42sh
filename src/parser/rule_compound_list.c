#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_compound_list(struct ast_node **result,
                                       struct sh_data *data)
{
    int rc;
    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    if ((rc = parse_and_or(result, data)) != PARSER_DONE)
        return rc;

    struct ast_node *and_or_node = NULL;
    while (lexer_consume(data->lexer, TOKEN_SEMICOLON)
           || lexer_consume(data->lexer, TOKEN_AND)
           || lexer_consume(data->lexer, TOKEN_NEWLINE))
    {
        while (lexer_consume(data->lexer, TOKEN_NEWLINE))
            continue;

        if ((rc = parse_and_or(&and_or_node, data)) == PARSER_ERROR)
            return PARSER_ERROR;
        if (rc == PARSER_NO_MATCH)
            return PARSER_DONE;

        struct ast_node_operator *op = ast_node_alloc(NODE_OPERATOR);
        op->operator_type = OPE_SEPARATOR;
        op->left = *result;
        op->right = and_or_node;

        *result = to_node(op);
    }

    return PARSER_DONE;
}
