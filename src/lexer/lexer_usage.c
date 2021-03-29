#include "lexer.h"
#include "lexer_internals.h"
#include "string_utils.h"
#include "token.h"

/**
 * @Synopsis  Returns and consumes the next token
 *
 * @Param lexer The lexer to lex from
 *
 * @Returns The next token
 */
struct token *lexer_pop(struct lexer *lexer)
{
    if (list_empty(lexer->token_list))
    {
        lexer_get_tokens(lexer);
        return lexer_pop(lexer);
    }

    return list_pop(lexer->token_list);
}

struct token *lexer_peek(struct lexer *lexer)
{
    if (list_empty(lexer->token_list))
    {
        lexer_get_tokens(lexer);
        return lexer_peek(lexer);
    }

    return list_peek(lexer->token_list);
}

void print_syntax_error(const char *function_name, struct lexer *lexer)
{
    fprintf(stderr, "%s: syntax error near unexpected token '%s'\n",
            function_name, lexer_peek(lexer)->value);
}
