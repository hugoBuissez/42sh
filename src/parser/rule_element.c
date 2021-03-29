#include <stdlib.h>
#include <string.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_element(struct argv_list *argument_list,
                                 struct list *redirection_list,
                                 struct sh_data *data)
{
    struct token *token = lexer_peek(data->lexer);
    struct redirection *redirection = NULL;

    int rc = PARSER_NO_MATCH;
    if (!token_type_is_word(token->type)
        && (rc = parse_redirection(&redirection, data)) != PARSER_DONE)
        return rc;

    if (redirection != NULL)
        list_append(redirection_list, redirection);
    else // WORD
    {
        argv_list_append(argument_list, token->value);
        token_destroy(lexer_pop(data->lexer));
    }

    return PARSER_DONE;
}
