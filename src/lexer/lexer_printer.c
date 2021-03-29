#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "lexer_internals.h"
#include "token.h"

void print_peek(struct lexer *lexer)
{
    struct token *token = lexer_peek(lexer);
    printf("%-22s: %s$\n", token_type_to_str(token->type), token->value);
}

void print_lexer(struct lexer *lexer)
{
    lexer_peek(lexer);
    struct list_iterator *iterator = iterator_init(lexer->token_list);

    struct token *token = NULL;
    int i = 0;
    while ((token = iterator_next(iterator)))
    {
        printf("%-3d %-22s: %s$\n", i++, token_type_to_str(token->type),
               token->value);
    }
    puts("");
    free(iterator);
}
