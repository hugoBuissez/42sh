#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_shell_command(struct ast_node **result,
                                       struct sh_data *data)
{
    int rc;
    if (lexer_consume(data->lexer, TOKEN_L_BR))
    {
        if ((rc = parse_compound_list(result, data)) != PARSER_DONE
            || !lexer_consume(data->lexer, TOKEN_R_BR))
            return not_done_error(rc, "parse_shell_command", data->lexer);
        return PARSER_DONE;
    }

    if (lexer_consume(data->lexer, TOKEN_L_PAR))
    {
        if ((rc = parse_compound_list(result, data)) != PARSER_DONE
            || !lexer_consume(data->lexer, TOKEN_R_PAR))
            return not_done_error(rc, "parse_shell_command", data->lexer);
        return PARSER_DONE;
    }

    if ((rc = parse_for(result, data)) != PARSER_NO_MATCH)
        return rc;
    if ((rc = parse_while(result, data)) != PARSER_NO_MATCH)
        return rc;
    if ((rc = parse_until(result, data)) != PARSER_NO_MATCH)
        return rc;
    if ((rc = parse_case(result, data)) != PARSER_NO_MATCH)
        return rc;
    if ((rc = parse_if(result, data)) != PARSER_NO_MATCH)
        return rc;

    return PARSER_NO_MATCH;
}
