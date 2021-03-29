#include <err.h>
#include <string.h>

#include "lexer_internals.h"
#include "lexer_rules.h"

static void lex_single_quote(struct lexer *lexer)
{
    next_sym(lexer);

    while (lexer->sym != '\'')
    {
        append_curr(lexer);
        next_sym(lexer);
    };
}

static void lex_double_quotes(struct lexer *lexer, enum reading_mode mode)
{
    do
    {
        next_sym(lexer);

        if (lexer->sym == EOF)
            errx(2, "unexpected EOF while looking for matching '\"'");

        if (lexer->sym == '\"' && !is_escaped(lexer))
            break;

        if (lexer->sym == '\\' && !is_escaped(lexer))
            rule_4(lexer, mode);

        else if (is_expansion(lexer->sym) && !is_escaped(lexer))
            rule_5(lexer, mode);

        else
            append_curr(lexer);

    } while (true);
}

static void lex_expansion(struct lexer *lexer, char *ending_seq,
                          enum reading_mode mode)
{
    do
    {
        next_sym(lexer);

        if (lexer->sym == EOF)
            errx(2, "unexpected EOF while looking for matching '%s'",
                 ending_seq);

        if (match_seq(lexer->ptr - 1, ending_seq) && !is_escaped(lexer))
            break;

        if (is_quoting(lexer->sym) && !is_escaped(lexer))
            rule_4(lexer, mode);

        else if (is_expansion(lexer->sym) && !is_escaped(lexer))
            rule_5(lexer, mode);

        else
            append_curr(lexer);

    } while (true);
}

static void lex_backtick_expansion(struct lexer *lexer)
{
    next_sym(lexer);

    while (lexer->sym != '`' || is_escaped(lexer))
    {
        if (lexer->sym == '\\')
            lexer->last_escaped = lexer->ptr; // Mark escaped character

        append_curr(lexer);
        next_sym(lexer);
    };
}

static void lex_arithmetic(struct lexer *lexer)
{
    int counter = 0;
    next_sym(lexer);
    append_curr(lexer);
    next_sym(lexer);
    append_curr(lexer);

    do
    {
        next_sym(lexer);

        if (lexer->sym == ')' && counter == 0)
            break;

        if (lexer->sym == '(')
            counter++;
        else if (lexer->sym == ')')
            counter--;

        append_curr(lexer);
    } while (true);

    append_curr(lexer);
    next_sym(lexer);
}

int rule_1(struct lexer *lexer)
{
    delim_token(lexer);
    list_append(lexer->token_list, token_create(TOKEN_EOF, "eof"));
    return 0;
}

void rule_2_3(struct lexer *lexer)
{
    append_curr(lexer);

    if (!is_valid_operator(string_value(lexer->curr_tok)))
    {
        string_pop_last_char(lexer->curr_tok);

        delim_token(lexer);
        prev_sym(lexer);
        lexer->state = IN_WORD;
    }
}

void rule_4(struct lexer *lexer, enum reading_mode mode)
{
    if (mode == TOKENISING && lexer->state != IN_WORD)
        delim_token(lexer);
    {
        lexer->state = IN_WORD;
    }

    append_curr(lexer);

    if (lexer->sym == '\\')
    {
        lexer->last_escaped = lexer->ptr; // Mark escaped character
        return;
    }

    if (lexer->sym == '\'')
        lex_single_quote(lexer);

    else if (lexer->sym == '\"')
        lex_double_quotes(lexer, mode);

    append_curr(lexer);

    lexer->sym = 0; // Make sure that when exiting no one matches the symbol
}

void rule_5(struct lexer *lexer, enum reading_mode mode)
{
    if (mode == TOKENISING && lexer->state != IN_WORD)
    {
        delim_token(lexer);
        lexer->state = IN_WORD;
    }

    append_curr(lexer);

    if (lexer->sym == '`')
        lex_backtick_expansion(lexer);

    else if (peek_next_sym(lexer) == '{')
        lex_expansion(lexer, "}", mode);

    else if (peek_next_sym(lexer) == '(' && *(lexer->ptr + 1) == '(')
        lex_arithmetic(lexer);

    else if (peek_next_sym(lexer) == '(')
        lex_expansion(lexer, ")", mode);

    else
        return;

    append_curr(lexer);

    lexer->sym = 0; // Make sure that when exiting no one matches the symbol
}

void rule_6(struct lexer *lexer)
{
    delim_token(lexer);
    append_curr(lexer);
    lexer->state = IN_OPERATOR;
}
