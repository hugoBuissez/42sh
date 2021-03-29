#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_add(struct lexer *lexer, struct ast **ast)
{
    if (!parse_mul(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek_a(lexer);
    while (res && token
           && (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS))
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOKEN_PLUS:
            tmp->type = EXPR_ADDITION;
            break;
        case TOKEN_MINUS:
            tmp->type = EXPR_SUBTRACTION;
            break;
        default:
            res = false;
            break;
        }
        res = parse_mul(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek_a(lexer);
    }

    return res;
}
