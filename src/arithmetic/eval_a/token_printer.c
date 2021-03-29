#include <err.h>
#include <stdio.h>

#include "lexer_a.h"
#include "token_a.h"

int print_tokens(char *input)
{
    /*
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }*/

    struct lexer *lexer = lexer_alloc(input);

    struct token *token = NULL;
    while ((token = lexer_pop_a(lexer)))
    {
        printf("%s\n", token_str(token));
        if (token->type == TOKEN_EOF)
            break;

        token_free(token);
        token = NULL;
    }

    token_free(token);
    lexer_free_a(lexer);
    return 1;
}
