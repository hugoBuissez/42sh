#include <err.h>
#include <stdio.h>

#include "ast_a.h"
#include "lexer_a.h"
#include "parser_a.h"

struct ast_char
{
    enum ast_type type;
    char c;
};

int ipow(int n, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= n;
        exp >>= 1;
        if (!exp)
            break;
        n *= n;
    }

    return result;
}

static int make_op(int a, int b, enum ast_type type)
{
    if (type == EXPR_ADDITION)
        return a + b;
    if (type == EXPR_POSITIVE)
        return b;
    if (type == EXPR_SUBTRACTION)
        return a - b;
    if (type == EXPR_MULTIPLICATION)
        return a * b;
    if (type == EXPR_DIVISION)
        return a / b;
    if (type == EXPR_POWER)
        return ipow(a, b);
    if (type == EXPR_MINUS)
        return -a;
    if (type == EXPR_AND)
        return a & b;
    if (type == EXPR_XOR)
        return a ^ b;
    if (type == EXPR_OR)
        return a | b;
    if (type == EXPR_LOGIC_AND)
        return a && b;
    if (type == EXPR_LOGIC_OR)
        return a || b;
    return ~a;
}

int eval_ast(struct ast *ast)
{
    if (!ast)
        return 0;

    if (ast->type == EXPR_NUMBER)
        return ast->data.value;

    eval_ast(ast->data.children.left);

    eval_ast(ast->data.children.right);

    return make_op(eval_ast(ast->data.children.left),
                   eval_ast(ast->data.children.right), ast->type);
}
