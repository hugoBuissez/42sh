#include <criterion/criterion.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ast_printer.h"
#include "parser.h"
#include "shell.h"
#include "token.h"

void test_ast_node_type(struct ast_node *node, enum ast_node_type expected)
{
    cr_expect_eq(node->type, expected, "Expected %d got %d", expected,
                 node->type);
}

Test(parser, valid_if, .timeout = 1)
{
    char *test = " if pwd; then echo toto; fi";
    FILE *stream = fmemopen(test, strlen(test), "r");
    struct sh_data data = { .lexer = lexer_init(stream) };

    struct ast_node *ast = NULL;
    enum parser_status status = parse_input(&ast, &data);

    cr_expect_eq(status, PARSER_EOF, "Expected %d got %d", PARSER_EOF, status);

    ast_node_free(ast);
    lexer_destroy(data.lexer);
    fclose(stream);
}

Test(parser, invalid_if, .timeout = 1)
{
    char *test = " if pwd then echo toto; fi";
    FILE *stream = fmemopen(test, strlen(test), "r");
    struct sh_data data = { .lexer = lexer_init(stream) };

    struct ast_node *ast = NULL;
    enum parser_status status = parse_input(&ast, &data);

    cr_expect_eq(status, PARSER_ERROR, "Expected %d got %d", PARSER_ERROR,
                 status);

    ast_node_free(ast);
    lexer_destroy(data.lexer);
    fclose(stream);
}

Test(parser, valid_for, .timeout = 1)
{
    char *test = "for i in 1 2 3 4 5; do echo foo; done";
    FILE *stream = fmemopen(test, strlen(test), "r");
    struct sh_data data = { .lexer = lexer_init(stream) };

    struct ast_node *ast = NULL;
    enum parser_status status = parse_input(&ast, &data);

    cr_expect_eq(status, PARSER_EOF, "Expected %d got %d", PARSER_EOF, status);

    ast_node_free(ast);
    lexer_destroy(data.lexer);
    fclose(stream);
}
