#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_unit(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_peek_a(lexer);
    if (!token)
        return false;

    bool res = true;
    if (token->type == TOKEN_LEFT_PARENTHESIS)
    {
        token = lexer_pop_a(lexer);
        res = parse_parenthesis(lexer, ast);
    }
    else if (token->type == TOKEN_NUMBER)
    {
        token = lexer_pop_a(lexer);
        *ast = ast_alloc_number(token->value);
    }
    else if (parse_negative(lexer, ast)) // '-'
        res = true;
    else if (parse_positive(lexer, ast)) // '+'
        res = true;
    else if (parse_bitwise_not(lexer, ast)) // '~'
        res = true;

    else
        res = false;

    token_free(token);

    return res;
}
