#include <criterion/criterion.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

void test_token_type(struct lexer *lexer, enum token_type expected)
{
    struct token *token = lexer_pop(lexer);

    cr_expect_eq(token->type, expected, "Expected %s got %s",
                 token_type_to_str(expected), token_type_to_str(token->type));

    token_destroy(token);
}

void test_token_value(struct lexer *lexer, char *expected)
{
    struct token *token = lexer_pop(lexer);

    cr_expect_eq(token->type, TOKEN_WORD, "Expected TOKEN_WORD got %s",
                 token_type_to_str(token->type));
    cr_expect_str_eq(token->value, expected, "Expected %s got %s", expected,
                     token->value);

    token_destroy(token);
}

void test_token_type_value(struct lexer *lexer, enum token_type exp_type,
                           char *exp_value)
{
    struct token *token = lexer_pop(lexer);

    cr_expect_eq(token->type, exp_type, "Expected %s got %s (%s)",
                 token_type_to_str(exp_type), token_type_to_str(token->type),
                 token->value);
    cr_expect_str_eq(token->value, exp_value, "Expected %s got %s", exp_value,
                     token->value);

    token_destroy(token);
}

void test_input(char *input, enum token_type expected_tokens[],
                size_t expected_len)
{
    FILE *stream = fmemopen(input, strlen(input), "r");
    struct lexer *lexer = lexer_init(stream);

    for (size_t i = 0; i < expected_len; i++)
        test_token_type(lexer, expected_tokens[i]);

    lexer_destroy(lexer);
}

Test(lexer, one_token, .timeout = 1)
{
    char *test = "hey";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_value(lexer, "hey");
    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, two_tokens, .timeout = 1)
{
    char *test = "Hello world";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_value(lexer, "Hello");
    test_token_value(lexer, "world");
    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, four_tokens, .timeout = 1)
{
    char *test = "   Hello world test psg    ";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_value(lexer, "Hello");
    test_token_value(lexer, "world");
    test_token_value(lexer, "test");
    test_token_value(lexer, "psg");
    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, test_eof, .timeout = 1)
{
    char *test = "";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, test_space_eof, .timeout = 1)
{
    char *test = "        ";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, test_separator, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_IF, TOKEN_SEMICOLON,
                                          TOKEN_EOF };
    char *input = "if;";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_all, .timeout = 1)
{
    char *test = "if;= do then foo";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_type(lexer, TOKEN_IF);
    test_token_type(lexer, TOKEN_SEMICOLON);
    test_token_value(lexer, "=");
    test_token_type(lexer, TOKEN_DO);
    test_token_type(lexer, TOKEN_THEN);
    test_token_type(lexer, TOKEN_WORD);
    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, test_all2, .timeout = 1)
{
    char *test = "do foo = ls then bar";
    FILE *stream = fmemopen(test, strlen(test), "r");

    struct lexer *lexer = lexer_init(stream);

    test_token_type(lexer, TOKEN_DO);
    test_token_type(lexer, TOKEN_WORD); // foo
    test_token_value(lexer, "=");
    test_token_type(lexer, TOKEN_WORD); // ls
    test_token_type(lexer, TOKEN_THEN);
    test_token_type(lexer, TOKEN_WORD); // bar
    test_token_type(lexer, TOKEN_EOF);

    lexer_destroy(lexer);
}

Test(lexer, test_type_test, .timeout = 1)
{
    enum token_type expected_tokens[] = {
        TOKEN_IF,   TOKEN_THEN,      TOKEN_WORD, TOKEN_AND_IF,
        TOKEN_WORD, TOKEN_SEMICOLON, TOKEN_FI,   TOKEN_EOF,
    };
    char *input = "   if then pwd     && ls ;  fi  ";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_consume, .timeout = 1)
{
    char *test = "   if pwd  && ls ; then fi  ";
    FILE *stream = fmemopen(test, strlen(test), "r");
    struct lexer *lexer = lexer_init(stream);

    cr_expect_eq(lexer_consume(lexer, TOKEN_IF), true);
    cr_expect_eq(lexer_consume(lexer, TOKEN_WORD), true);
    cr_expect_eq(lexer_consume(lexer, TOKEN_FI), false);

    lexer_destroy(lexer);
}

Test(lexer, test_redirections, .timeout = 1)
{
    enum token_type expected_tokens[] = {
        TOKEN_WORD,      TOKEN_WORD,     TOKEN_REDIRECT, TOKEN_WORD,
        TOKEN_IO_NUMBER, TOKEN_REDIRECT, TOKEN_WORD,
    };
    char *input = "echo hey > redirect.txt 2< 3";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_assignements, .timeout = 1)
{
    char *test = "foo=bar ; _foo=_bar ; f=b ;";
    FILE *stream = fmemopen(test, strlen(test), "r");
    struct lexer *lexer = lexer_init(stream);

    test_token_type_value(lexer, TOKEN_ASSIGNMENT_WORD, "foo=bar");
    test_token_type(lexer, TOKEN_SEMICOLON);
    test_token_type_value(lexer, TOKEN_ASSIGNMENT_WORD, "_foo=_bar");
    test_token_type(lexer, TOKEN_SEMICOLON);
    test_token_type_value(lexer, TOKEN_ASSIGNMENT_WORD, "f=b");
    test_token_type(lexer, TOKEN_SEMICOLON);

    lexer_destroy(lexer);
}

Test(lexer, test_while, .timeout = 1)
{
    enum token_type expected_tokens[] = {
        TOKEN_WHILE, TOKEN_WORD, TOKEN_SEMICOLON, TOKEN_DO,
        TOKEN_WORD,  TOKEN_WORD, TOKEN_SEMICOLON, TOKEN_DONE,
    };
    char *input = "while pwd; do echo foo; done";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_until, .timeout = 1)
{
    enum token_type expected_tokens[] = {
        TOKEN_UNTIL, TOKEN_WORD, TOKEN_AND_IF, TOKEN_WORD,      TOKEN_SEMICOLON,
        TOKEN_DO,    TOKEN_WORD, TOKEN_WORD,   TOKEN_SEMICOLON, TOKEN_DONE,
    };
    char *input = "until pwd && ls; do echo foo; done";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_for, .timeout = 1)
{
    enum token_type expected_tokens[] = {
        TOKEN_FOR,  TOKEN_WORD, TOKEN_WORD,      TOKEN_WORD,
        TOKEN_WORD, TOKEN_WORD, TOKEN_SEMICOLON, TOKEN_DO,
        TOKEN_WORD, TOKEN_WORD, TOKEN_SEMICOLON, TOKEN_DONE,
    };
    char *input = "for i in 1 2 3; do echo foo; done";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_quoting_1, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_WORD, TOKEN_WORD, TOKEN_EOF };
    char *input = "echo \" echo foo $(echo \"bar)\") \"";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_quoting_2, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_WORD, TOKEN_WORD, TOKEN_EOF };
    char *input = "echo \" foo $(echo \"bar\\)\") \"";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_quoting_4, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_WORD, TOKEN_WORD, TOKEN_EOF };
    char *input = "echo \"foo $(echo 'bar' \"echo wow\") $(echo test)\"";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_quoting_5, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_WORD, TOKEN_WORD, TOKEN_EOF };
    char *input = "echo \\thisisa\"test\"$(echo bar)";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}

Test(lexer, test_quoting_6, .timeout = 1)
{
    enum token_type expected_tokens[] = { TOKEN_WORD, TOKEN_WORD, TOKEN_EOF };
    char *input = "echo \"foo 'how about this\"'\"'";
    test_input(input, expected_tokens,
               sizeof(expected_tokens) / sizeof(expected_tokens[0]));
}
