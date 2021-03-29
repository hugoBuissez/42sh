#include <err.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_do_group(struct ast_node **result,
                                  struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_DO))
        return syntax_error("parse_do_group", data->lexer);

    if ((rc = parse_compound_list(result, data)) != PARSER_DONE)
        return rc;

    if (!lexer_consume(data->lexer, TOKEN_DONE))
        return syntax_error("parse_do_group", data->lexer);

    return PARSER_DONE;
}
