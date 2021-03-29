#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_positive(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_peek_a(lexer);

    bool res = true;
    if (token->type == TOKEN_PLUS) // '+'
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();

        tmp->type = EXPR_POSITIVE;
        res = parse_logic_or(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
    }
    else
        res = false;

    return res;
}
