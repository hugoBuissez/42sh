#include <err.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_funcdec(char **func_name, struct ast_node **func_body,
                                 struct sh_data *data)
{
    lexer_consume_specific_word(data->lexer, "function");

    int rc;

    if (!lexer_consume_word(data->lexer, TOKEN_WORD, func_name))
        return PARSER_NO_MATCH;

    if (!lexer_consume(data->lexer, TOKEN_L_PAR))
        return syntax_error("parse_funcdec", data->lexer);

    if (!lexer_consume(data->lexer, TOKEN_R_PAR))
        return syntax_error("parse_funcdec", data->lexer);

    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    if ((rc = parse_shell_command(func_body, data)) != PARSER_DONE)
        return not_done_error(rc, "parse_funcdec", data->lexer);

    return PARSER_DONE;
}
