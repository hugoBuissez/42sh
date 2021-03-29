#include <stdlib.h>
#include <string.h>

#include "lexer_internals.h"
#include "token.h"

struct token_model
{
    char *value;
    enum token_type type;
};

static bool is_io_number(const char *word)
{
    size_t i = 0;
    for (; word[i] && isdigit(word[i]); i++)
        continue;
    return word[i] == '\0';
}

static bool is_valid_name(const char *word)
{
    if (!valid_name_start(word[0]))
        return 0;

    size_t i = 1;
    for (; word[i] && valid_name_char(word[i]); i++)
        continue;
    return word[i] == '\0';
}

static struct token *match_operator(char *word)
{
    static struct token_model operator[] = {
        { "&&", TOKEN_AND_IF }, { "||", TOKEN_OR_IF },    { ";;", TOKEN_DSEMI },
        { "&", TOKEN_AND },     { ";", TOKEN_SEMICOLON }, { "|", TOKEN_PIPE },
        { "(", TOKEN_L_PAR },   { ")", TOKEN_R_PAR },
    };

    for (size_t i = 0; i < sizeof(operator) / sizeof(operator[0]); i++)
        if (strcmp(word, operator[i].value) == 0)
            return token_create(operator[i].type, operator[i].value);

    return NULL;
}

static struct token *match_redir(char *word)
{
    static char *redirects[] = {
        "<<-", "<<", ">>", ">|", "<&", ">&", "<>", "<", ">",
    };

    for (size_t i = 0; i < sizeof(redirects) / sizeof(redirects[0]); i++)
        if (strcmp(word, redirects[i]) == 0)
            return token_create(TOKEN_REDIRECT, redirects[i]);

    return NULL;
}

static struct token *match_reserved_word(char *word)
{
    static struct token_model reserved[] = {
        { "if", TOKEN_IF },       { "then", TOKEN_THEN },
        { "else", TOKEN_ELSE },   { "elif", TOKEN_ELIF },
        { "fi", TOKEN_FI },       { "do", TOKEN_DO },
        { "done", TOKEN_DONE },   { "while", TOKEN_WHILE },
        { "until", TOKEN_UNTIL }, { "for", TOKEN_FOR },
        { "case", TOKEN_CASE },   { "esac", TOKEN_ESAC },
        { "{", TOKEN_L_BR },      { "}", TOKEN_R_BR },
        { "!", TOKEN_NOT },
    };

    for (size_t i = 0; i < sizeof(reserved) / sizeof(reserved[0]); i++)
        if (strcmp(word, reserved[i].value) == 0)
            return token_create(reserved[i].type, reserved[i].value);

    return NULL;
}

struct token *create_token_from_word(char *word, char delim)
{
    struct token *token = NULL;

    if ((token = match_operator(word)))
        return token;
    if ((token = match_redir(word)))
        return token;
    if ((token = match_reserved_word(word)))
        return token;

    // Check if io_number
    if (is_io_number(word) && is_redirect_prefix(delim))
        return token_create(TOKEN_IO_NUMBER, word);

    // Check if assignment word
    char *eq_ptr = strchr(word, '=');
    if (eq_ptr == NULL || eq_ptr == word)
        return token_create(TOKEN_WORD, word);

    char *variable_name = strndup(word, eq_ptr - word);
    bool valid_name = is_valid_name(variable_name);
    free(variable_name);
    if (valid_name)
        return token_create(TOKEN_ASSIGNMENT_WORD, word);

    return token_create(TOKEN_WORD, word);
}
