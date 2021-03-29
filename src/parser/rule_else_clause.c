#include <err.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_else_clause(struct ast_node **result,
                                     struct sh_data *data)
{
    int rc;

    if (lexer_consume(data->lexer, TOKEN_ELSE))
    {
        if ((rc = parse_compound_list(result, data)) == PARSER_ERROR)
            return PARSER_ERROR;
        if (rc == PARSER_NO_MATCH)
            return syntax_error("parse_else_clause", data->lexer);
        return PARSER_DONE;
    }

    if (lexer_consume(data->lexer, TOKEN_ELIF))
    {
        struct ast_node_if *elif = ast_node_alloc_attach(NODE_IF, result);
        if ((rc = parse_compound_list(&(elif->condition), data)) != PARSER_DONE)
            return not_done_error(rc, "parse_else_clause", data->lexer);

        if (!lexer_consume(data->lexer, TOKEN_THEN))
            return syntax_error("parse_else_clause", data->lexer);

        if ((rc = parse_compound_list(&(elif->if_body), data)) != PARSER_DONE)
            return not_done_error(rc, "parse_else_clause", data->lexer);

        if (parse_else_clause(&(elif->else_body), data) == PARSER_ERROR)
            return PARSER_ERROR;
    }

    return PARSER_DONE;
}
