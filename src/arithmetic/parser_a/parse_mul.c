#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_mul(struct lexer *lexer, struct ast **ast)
{
    if (!parse_expo(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek_a(lexer);
    while (res && token
           && (token->type == TOKEN_MULTIPLY || token->type == TOKEN_DIVIDE))
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOKEN_MULTIPLY:
            tmp->type = EXPR_MULTIPLICATION;
            break;
        case TOKEN_DIVIDE:
            tmp->type = EXPR_DIVISION;
            break;
        default:
            res = false;
            break;
        }
        res = parse_expo(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek_a(lexer);
    }

    return res;
}
