#include <stdlib.h>
#include <string.h>

#include "ast_printer.h"
#include "list.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_prefix(struct argv_list *assignments,
                                struct list *redirection_list,
                                struct sh_data *data)
{
    struct token *token = lexer_peek(data->lexer);
    struct redirection *redirection = NULL;

    int rc = PARSER_NO_MATCH;
    if (token->type != TOKEN_ASSIGNMENT_WORD
        && (rc = parse_redirection(&redirection, data)) != PARSER_DONE)
        return rc;

    if (redirection != NULL)
        list_append(redirection_list, redirection);
    else // ASSIGNMENT_WORD
    {
        struct token *var_token = lexer_pop(data->lexer);
        argv_list_append(assignments, token->value);
        token_destroy(var_token);
    }

    return PARSER_DONE;
}
