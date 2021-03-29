#include <err.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_and_or(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if ((rc = parse_pipeline(result, data)) != PARSER_DONE)
        return rc; // NO_MATCH or ERROR

    bool and = false;

    while ((and = lexer_consume(data->lexer, TOKEN_AND_IF))
           || lexer_consume(data->lexer, TOKEN_OR_IF))
    {
        while (lexer_consume(data->lexer, TOKEN_NEWLINE))
            continue;

        struct ast_node *pipeline_node = NULL;
        if ((rc = parse_pipeline(&pipeline_node, data)) != PARSER_DONE)
            return not_done_error(rc, "parse_and_or", data->lexer);

        struct ast_node_operator *op = ast_node_alloc(NODE_OPERATOR);
        op->operator_type = and? OPE_AND : OPE_OR;
        op->left = *result;
        op->right = pipeline_node;

        *result = to_node(op);

        and = false;
    }

    return PARSER_DONE;
}
