#include "lexer_a.h"
#include "parser_a.h"
#include "xalloc.h"

bool parse_logic_or(struct lexer *lexer, struct ast **ast)
{
    if (!parse_logic_and(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek_a(lexer);
    while (res && token && token->type == TOKEN_LOGIC_OR)
    {
        token = lexer_pop_a(lexer);

        struct ast *tmp = ast_alloc();
        tmp->type = EXPR_LOGIC_OR;
        res = parse_logic_and(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek_a(lexer);
    };

    return res;
}
