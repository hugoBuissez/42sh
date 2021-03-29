#include "lexer_a.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer_struct.h"
#include "xalloc.h"

struct token_model
{
    char *value;
    enum token_type type;
};

static struct lexer *lexer_init(enum token_type type, int value)
{
    struct lexer *res = xmalloc(sizeof(*res));

    struct token *token = xmalloc(sizeof(*token));

    res->token = token;
    res->token->type = type;
    res->token->value = value;
    res->next = NULL;
    res->consumed = 0;
    return res;
}

static struct lexer *insert_lexer(struct lexer *l, enum token_type type,
                                  int value)
{
    struct lexer *new = lexer_init(type, value);

    if (!l)
        return new;

    struct lexer *first = l;

    while (l->next)
        l = l->next;

    l->next = new;
    return first;
}

static enum token_type get_type(const char **str)
{
    if (**str >= '0' && **str <= '9')
        return TOKEN_NUMBER;

    static struct token_model operator[] = {
        { "+", TOKEN_PLUS },
        { "-", TOKEN_MINUS },
        { "**", TOKEN_POWER },
        { "/", TOKEN_DIVIDE },
        { "*", TOKEN_MULTIPLY },
        { "&&", TOKEN_LOGIC_AND },
        { "||", TOKEN_LOGIC_OR },
        { "^", TOKEN_XOR },
        { "&", TOKEN_AND },
        { "|", TOKEN_OR },
        { "!", TOKEN_NOT },
        { "~", TOKEN_BITWISE_NOT },
        { "(", TOKEN_LEFT_PARENTHESIS },
        { ")", TOKEN_RIGHT_PARENTHESIS },
    };

    if (**str == 0)
        return TOKEN_EOF;

    for (size_t i = 0; i < sizeof(operator) / sizeof(operator[0]); i++)
    {
        if (strstr(*str, operator[i].value) == *str)
        {
            *str += strlen(operator[i].value);
            return operator[i].type;
        }
    }
    return TOKEN_INVALID;
}

static int get_number(const char **str)
{
    int res = 0;
    while (**str >= '0' && **str <= '9')
    {
        res *= 10;
        res += **str - '0';
        (*str)++;
    }
    return res;
}

static struct lexer *parse_current(const char **str, struct lexer *l)
{
    enum token_type type = get_type(str);

    int value = 0;
    if (type == TOKEN_NUMBER)
        value = get_number(str);
    else
    {
        if (type == TOKEN_INVALID)
            value = *(*str)++;
    }
    l = insert_lexer(l, type, value);
    return l;
}

static void skip_spaces(const char **str)
{
    while (*str && (**str == ' ' || **str == '\t' || **str == '\n'))
        (*str)++;
}

struct lexer *lexer_alloc(const char *str)
{
    struct lexer *res = NULL;

    skip_spaces(&str);
    while (*str)
    {
        res = parse_current(&str, res);
        if (!res)
            return NULL;
        skip_spaces(&str);
    }
    res = insert_lexer(res, TOKEN_EOF, 0);
    return res;
}

void lexer_free_a(struct lexer *lexer)
{
    if (!lexer)
        return;
    lexer_free_a(lexer->next);
    if (lexer->consumed == 0)
        token_free(lexer->token);

    free(lexer);
}

struct token *lexer_peek_a(struct lexer *lexer)
{
    if (!lexer)
        return NULL;
    while (lexer && lexer->consumed)
        lexer = lexer->next;

    if (lexer == NULL)
        return NULL;
    return lexer->token;
}

struct token *lexer_pop_a(struct lexer *lexer)
{
    while (lexer && lexer->consumed)
        lexer = lexer->next;

    if (!lexer)
        return NULL;

    if (lexer->token->type == TOKEN_INVALID)
    {
        fprintf(stderr, "lexer: token: %c is not valid\n", lexer->token->value);
        return NULL;
    }

    lexer->consumed = 1;
    return lexer->token;
}
