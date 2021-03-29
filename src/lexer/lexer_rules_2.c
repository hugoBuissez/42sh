#include "lexer_internals.h"
#include "lexer_rules.h"

int rule_7(struct lexer *lexer)
{
    if (is_escaped(lexer)) // Line continuation
        return 0;

    delim_token(lexer);
    list_append(lexer->token_list, token_create(TOKEN_NEWLINE, "newline"));
    return 0;
}

void rule_8(struct lexer *lexer)
{
    if (is_escaped(lexer))
        return; // Skip '\\n'

    delim_token(lexer);
    lexer->state = IN_WHITESPACE;
}

void rule_9(struct lexer *lexer)
{
    append_curr(lexer);
}

void rule_10(struct lexer *lexer)
{
    while (lexer->sym != '\n')
        next_sym(lexer);
    prev_sym(lexer);
}

void rule_11(struct lexer *lexer)
{
    append_curr(lexer);
    lexer->state = IN_WORD;
}
