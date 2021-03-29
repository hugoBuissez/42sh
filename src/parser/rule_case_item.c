#include <err.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_case_item(struct case_item **result,
                                   struct sh_data *data)
{
    lexer_consume(data->lexer, TOKEN_L_PAR);

    char *word = NULL;
    if (!lexer_consume_word(data->lexer, TOKEN_WORD, &word))
        return PARSER_NO_MATCH;

    struct case_item *case_item = calloc(1, sizeof(*case_item));
    *result = case_item;
    case_item->elements = argv_list_init();

    argv_list_append(case_item->elements, word);
    free(word);

    while (lexer_consume(data->lexer, TOKEN_PIPE))
    {
        if (!lexer_consume_any_word(data->lexer, &word))
            return syntax_error("parse_case_item", data->lexer);

        argv_list_append(case_item->elements, word);
        free(word);
    }

    if (!lexer_consume(data->lexer, TOKEN_R_PAR))
        return syntax_error("parse_case_item", data->lexer);

    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    parse_compound_list(&(case_item->body), data);
    return PARSER_DONE;
}

void free_case_item(void *_case_item)
{
    struct case_item *case_item = _case_item;
    if (case_item == NULL)
        return;

    argv_list_free(case_item->elements);

    ast_node_free(case_item->body);
    free(case_item);
}

void print_case_item(FILE *file, struct case_item *case_item)
{
    if (case_item == NULL)
        return;

    fprintf(file, "\"");

    struct argv_list *elements = case_item->elements;
    for (size_t i = 0; i < elements->len; i++)
        fprintf(file, "%s ", elements->argv[i]);

    fprintf(file, "\"");
}
