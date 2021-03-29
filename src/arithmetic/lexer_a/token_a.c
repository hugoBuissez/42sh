#include "token_a.h"

#include <stdio.h>
#include <stdlib.h>

#include "xalloc.h"

const char *token_str(struct token *token)
{
    static const char *token_strs[] = { "+", "-", "*", "/",  "**",
                                        "&", "|", "^", "&&", "||",
                                        "!", "~", "(", ") ", "EOF" };
    static char number_str[14];

    if (token->type != TOKEN_NUMBER)
        return token_strs[token->type];

    sprintf(number_str, "%d", token->value);
    return number_str;
}

struct token *token_alloc(void)
{
    struct token *res = xmalloc(sizeof(*res));
    res->type = TOKEN_EOF;
    res->value = 0;

    return res;
}

void token_free(struct token *token)
{
    free(token);
}
