#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_parenthesis(struct lexer *lexer, struct ast **ast)
{
    bool res = parse_logic_or(lexer, ast);

    struct token *token = lexer_pop_a(lexer);
    res = res && token && token->type == TOKEN_RIGHT_PARENTHESIS;

    token_free(token);
    return res;
}
