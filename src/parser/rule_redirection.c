#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "ast_printer.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_redirection(struct redirection **result,
                                     struct sh_data *data)
{
    char *io_number = NULL;
    bool parsed_io =
        lexer_consume_word(data->lexer, TOKEN_IO_NUMBER, &io_number);

    char *redirect = NULL;
    bool parsed_redirect =
        lexer_consume_word(data->lexer, TOKEN_REDIRECT, &redirect);

    if (!parsed_io && !parsed_redirect)
    {
        free(io_number);
        free(redirect);
        return PARSER_NO_MATCH;
    }

    if (parsed_io && !parsed_redirect)
    {
        free(io_number);
        free(redirect);
        return syntax_error("parse_redirection", data->lexer);
    }

    char *word = NULL;
    if (!lexer_consume_any_word(data->lexer, &word))
    {
        free(io_number);
        free(redirect);
        return syntax_error("parse_redirection", data->lexer);
    }

    struct redirection *redirection = calloc(1, sizeof(*redirection));
    *result = redirection;
    redirection->io_number = io_number;
    redirection->operator= redirect;
    redirection->word = word;

    return PARSER_DONE;
}

void free_redirection(void *_redirection)
{
    if (_redirection == NULL)
        return;

    struct redirection *redirection = _redirection;
    free(redirection->io_number);
    free(redirection->operator);
    free(redirection->word);

    free(redirection);
}

void print_redirection(FILE *file, struct redirection *redirection)
{
    if (redirection == NULL)
        return;

    if (redirection->io_number)
        fprintf(file, " %s", redirection->io_number);
    fprintf(file, " %s %s", redirection->operator, redirection->word);
}
