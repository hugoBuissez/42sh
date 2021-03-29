#include <err.h>
#include <stdlib.h>

#include "dynamic_string.h"
#include "lexer.h"
#include "lexer_internals.h"
#include "token.h"

struct lexer *lexer_init(FILE *stream)
{
    struct lexer *lexer = calloc(1, sizeof(*lexer));
    if (lexer == NULL)
        errx(1, "Memory exhausted");

    lexer->stream_in = stream;
    lexer->token_list = list_init();

    lexer->curr_tok = string_create("");

    if (stream == NULL)
        list_push(lexer->token_list, token_create(TOKEN_EOF, "eof"));

    return lexer;
}

/**
 * @Synopsis Free the given lexer
 *
 * @Param lexer The lexer to free
 */
void lexer_destroy(struct lexer *lexer)
{
    free(lexer->line);

    list_free_elements(lexer->token_list, token_destroy);
    list_destroy(lexer->token_list);

    string_destroy(lexer->curr_tok);

    free(lexer);
}
