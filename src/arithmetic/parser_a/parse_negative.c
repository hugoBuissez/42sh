#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

static bool setup_tree(struct token *token, struct lexer *lexer,
                       struct ast **ast)
{
    struct ast *tmp = ast_alloc();

    tmp->type = EXPR_MINUS;
    token = lexer_pop_a(lexer);

    bool res = parse_parenthesis(lexer, &tmp->data.children.left);
    tmp->data.children.right = *ast;
    *ast = tmp;
    token_free(token);

    return res;
}

bool parse_negative(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_peek_a(lexer);

    bool res = true;
    if (token->type == TOKEN_MINUS)
    {
        token = lexer_pop_a(lexer);
        token = lexer_peek_a(lexer);

        if (token && token->type == TOKEN_NUMBER)
        {
            token = lexer_pop_a(lexer);
            *ast = ast_alloc_number(-token->value);
            token_free(token);
        }
        else if (token && token->type == TOKEN_LEFT_PARENTHESIS)
            res = setup_tree(token, lexer, ast);

        else
        {
            struct ast *tmp = ast_alloc();

            tmp->type = EXPR_MINUS;
            res = parse_logic_or(lexer, &tmp->data.children.left);
            tmp->data.children.right = *ast;
            *ast = tmp;
        }
    }
    else
        res = false;

    return res;
}
