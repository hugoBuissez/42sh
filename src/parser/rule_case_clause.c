#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_case_clause(struct list *case_item_list,
                                     struct sh_data *data)
{
    int rc;

    struct case_item *case_item = NULL;
    if ((rc = parse_case_item(&case_item, data)) != PARSER_DONE)
    {
        free_case_item(case_item);
        return rc;
    }

    list_append(case_item_list, case_item);

    while (lexer_consume(data->lexer, TOKEN_DSEMI))
    {
        while (lexer_consume(data->lexer, TOKEN_NEWLINE))
            continue;

        if ((rc = parse_case_item(&case_item, data)) == PARSER_ERROR)
        {
            free_case_item(case_item);
            return PARSER_ERROR;
        }
        if (rc == PARSER_NO_MATCH)
            return PARSER_DONE;

        list_append(case_item_list, case_item);
    }

    return PARSER_DONE;
}
