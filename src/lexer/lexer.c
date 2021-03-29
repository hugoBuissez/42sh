#include "lexer.h"

#include <err.h>
#include <string.h>
#include <sys/types.h>

#include "lexer_internals.h"
#include "lexer_rules.h"

char next_sym(struct lexer *lexer)
{
    if (lexer->ptr && *(lexer->ptr))
    {
        lexer->sym = *(lexer->ptr)++;
        return lexer->sym;
    }

    if (get_new_line(lexer) == EOF)
    {
        lexer->sym = EOF;
        return lexer->sym;
    }

    return next_sym(lexer);
}

void prev_sym(struct lexer *lexer)
{
    if (lexer->ptr > lexer->line)
        (lexer->ptr)--;
}

int lexer_get_tokens(struct lexer *lexer)
{
    lexer->state = UNKNOWN;
    lexer->last_escaped = lexer->ptr - 2;

    // Follows Token Recognition (SCL 2.3)
    while (next_sym(lexer))
    {
        if (lexer->sym == EOF)
            return rule_1(lexer);

        if (lexer->state == IN_OPERATOR && !is_escaped(lexer))
            rule_2_3(lexer);

        else if (is_quoting(lexer->sym) && !is_escaped(lexer))
            rule_4(lexer, TOKENISING);

        else if (is_expansion(lexer->sym) && !is_escaped(lexer))
            rule_5(lexer, TOKENISING);

        else if (first_char_of_op(lexer->sym) && !is_escaped(lexer))
            rule_6(lexer);

        else if (lexer->sym == '\n')
            return rule_7(lexer);

        else if (isblank(lexer->sym))
            rule_8(lexer);

        else if (lexer->state == IN_WORD)
            rule_9(lexer);

        else if (lexer->sym == '#' && !is_escaped(lexer))
            rule_10(lexer);

        else
            rule_11(lexer);
    }

    return 0;
}
