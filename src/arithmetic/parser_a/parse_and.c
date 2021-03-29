#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_and(struct lexer *lexer, struct ast **ast)
{
    if (!parse_add(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek_a(lexer);
    while (res && token && token->type == TOKEN_AND)
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();

        tmp->type = EXPR_AND;
        res = parse_add(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek_a(lexer);
    };

    return res;
}
