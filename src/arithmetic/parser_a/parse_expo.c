#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_expo(struct lexer *lexer, struct ast **ast)
{
    if (!parse_unit(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek_a(lexer);
    while (res && token && token->type == TOKEN_POWER)
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOKEN_POWER:
            tmp->type = EXPR_POWER;
            break;
        default:
            res = false;
            break;
        }
        res = parse_unit(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek_a(lexer);
    };

    return res;
}
