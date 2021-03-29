#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_eval_a.h"
#include "lexer_a.h"
#include "parser_a.h"
#include "token_a.h"
#include "token_printer.h"

char *eval_arithmetic_expr(char *input)
{
    // print_tokens(input);
    struct lexer *lexer = lexer_alloc(input);
    struct ast *ast = NULL;

    char *result = NULL;

    if (parse_input_a(lexer, &ast))
    {
        int res = eval_ast(ast);
        result = calloc(16, sizeof(char));
        sprintf(result, "%d", res);
    }
    else
        warnx("arithmetic syntax error");

    token_free(lexer_pop_a(lexer));
    ast_free(ast);
    lexer_free_a(lexer);

    return result;
}
