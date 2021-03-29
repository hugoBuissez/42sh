#include <string.h>

#include "lexer.h"
#include "lexer_internals.h"
#include "list.h"
#include "token.h"

bool lexer_consume(struct lexer *lexer, enum token_type type)
{
    if (lexer_peek(lexer)->type == type)
    {
        token_destroy(lexer_pop(lexer));
        return true;
    }
    return false;
}

bool lexer_consume_word(struct lexer *lexer, enum token_type type,
                        char **ret_word)
{
    struct token *token = lexer_peek(lexer);

    if (token == NULL || token->type != type)
        return false;

    *ret_word = strdup(token->value);

    lexer_pop(lexer);
    token_destroy(token);

    return true;
}

bool lexer_consume_any_word(struct lexer *lexer, char **ret_word)
{
    struct token *token = lexer_peek(lexer);

    if (token == NULL || !token_type_is_word(token->type))
        return false;

    *ret_word = strdup(token->value);

    token_destroy(lexer_pop(lexer));

    return true;
}

bool lexer_consume_specific_word(struct lexer *lexer, char *specific_word)
{
    struct token *token = lexer_peek(lexer);

    if (token == NULL || token->value == NULL
        || strcmp(specific_word, token->value) != 0)
        return false;

    token_destroy(lexer_pop(lexer));

    return true;
}

static bool pop_type(struct lexer *lexer, enum token_type type,
                     struct token **result)
{
    if (lexer_peek(lexer)->type == type)
    {
        *result = lexer_pop(lexer);
        return true;
    }
    return false;
}

static bool push_back_elements(struct lexer *lexer, struct token *function,
                               struct token *fun_name, struct token *left_par)
{
    if (left_par != NULL)
        list_push(lexer->token_list, left_par);
    if (fun_name != NULL)
        list_push(lexer->token_list, fun_name);
    if (function != NULL)
        list_push(lexer->token_list, function);
    return false;
}

bool lexer_is_funcdec(struct lexer *lexer)
{
    struct token *function = NULL;
    struct token *fun_name = NULL;
    struct token *left_par = NULL;

    if (lexer_peek(lexer)->value == NULL)
        return false;

    if (strcmp(lexer_peek(lexer)->value, "function") == 0)
        function = lexer_pop(lexer);

    if (!pop_type(lexer, TOKEN_WORD, &fun_name))
        return push_back_elements(lexer, function, fun_name, left_par);

    if (!pop_type(lexer, TOKEN_L_PAR, &left_par))
        return push_back_elements(lexer, function, fun_name, left_par);

    if (lexer_peek(lexer)->type != TOKEN_R_PAR)
        return push_back_elements(lexer, function, fun_name, left_par);

    push_back_elements(lexer, function, fun_name, left_par);

    return true;
}
