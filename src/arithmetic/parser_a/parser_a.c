#include "parser_a.h"

#include "lexer_a.h"
#include "xalloc.h"

/**
 *
 *   input:      log_or EOF
 *           |   EOF
 *
 *   log_or:     log_and ('||' log_and)*
 *
 *   log_and:    or ('&&' or)*
 *
 *   or:         xor ('|' xor)*
 *
 *   xor:        and ('^' and)*
 *
 *   and:        add ('&' add)*
 *
 *   add:        mul ('+' mul | '-' mul)*
 *
 *   mul:        expo ('*' expo | '/' expo)*
 *
 *   expo:       unit ('**' unit)*
 *
 *
 *   unit:       NUMBER
 *           |   negative
 *           |   positive
 *           |   '(' log_or ')'
 *
 *   negative:   '-' log_or
 *
 *   positive:   '+' log_or
 *
 */

bool parse_input_a(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_peek_a(lexer);
    if (!token || token->type == TOKEN_EOF)
        return false;

    if (!parse_logic_or(lexer, ast))
        return false;

    token = lexer_peek_a(lexer);
    return token && token->type == TOKEN_EOF;
}
