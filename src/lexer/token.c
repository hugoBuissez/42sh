#include "token.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool token_type_is_word(enum token_type type)
{
    return type <= TOKEN_WORD;
}

/**
 * @Synopsis Allocates a new token
 *
 * @Returns The new token
 */
struct token *token_create(enum token_type type, char *value)
{
    struct token *token = calloc(1, sizeof(*token));
    if (token == NULL)
        errx(1, "Memory exhausted");

    token->type = type;
    token->value = strdup(value);

    return token;
}

void print_token(struct token *token)
{
    printf("%-22s: %s$\n", token_type_to_str(token->type), token->value);
}

/**
 * @Synopsis Free the given token
 *
 * @Param token The token to free
 */
void token_destroy(void *_token)
{
    if (_token == NULL)
        return;

    struct token *token = _token; // cast to access fields
    free(token->value);
    free(token);
}

static const char *reserved_to_str(enum token_type type)
{
    static const char *type_str[] = {
        [TOKEN_NOT] = "TOKEN_NOT",     [TOKEN_L_BR] = "TOKEN_L_BR",
        [TOKEN_R_BR] = "TOKEN_R_BR",   [TOKEN_CASE] = "TOKEN_CASE",
        [TOKEN_DO] = "TOKEN_DO",       [TOKEN_DONE] = "TOKEN_DONE",
        [TOKEN_ELIF] = "TOKEN_ELIF",   [TOKEN_ELSE] = "TOKEN_ELSE",
        [TOKEN_ESAC] = "TOKEN_ESAC",   [TOKEN_FI] = "TOKEN_FI",
        [TOKEN_FOR] = "TOKEN_FOR",     [TOKEN_IF] = "TOKEN_IF",
        [TOKEN_IN] = "TOKEN_IN",       [TOKEN_THEN] = "TOKEN_THEN",
        [TOKEN_UNTIL] = "TOKEN_UNTIL", [TOKEN_WHILE] = "TOKEN_WHILE",
    };

    return type_str[type];
}

const char *token_type_to_str(enum token_type type)
{
    if (type <= TOKEN_ESAC)
        return reserved_to_str(type);

    static const char *type_str[] = {
        [TOKEN_ASSIGNMENT_WORD] = "TOKEN_ASSIGNMENT_WORD",
        [TOKEN_WORD] = "TOKEN_WORD",
        // -----
        [TOKEN_AND_IF] = "TOKEN_AND_IF",
        [TOKEN_OR_IF] = "TOKEN_OR_IF",
        [TOKEN_DSEMI] = "TOKEN_DSEMI",
        [TOKEN_AND] = "TOKEN_AND",
        [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
        [TOKEN_PIPE] = "TOKEN_PIPE",
        [TOKEN_L_PAR] = "TOKEN_L_PAR",
        [TOKEN_R_PAR] = "TOKEN_R_PAR",
        [TOKEN_IO_NUMBER] = "TOKEN_IO_NUMBER",
        [TOKEN_REDIRECT] = "TOKEN_REDIRECT",
        // -----
        [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
        [TOKEN_EOF] = "TOKEN_EOF",
    };

    return type_str[type];
}
