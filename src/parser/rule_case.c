#include <err.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_case(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_CASE))
        return PARSER_NO_MATCH;

    struct ast_node_case *case_node = ast_node_alloc_attach(NODE_CASE, result);
    case_node->case_item_list = list_init();

    if (!(lexer_consume_word(data->lexer, TOKEN_WORD, &(case_node->var))))
        return syntax_error("parse_case", data->lexer);

    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    if (!lexer_consume_specific_word(data->lexer, "in"))
        return syntax_error("parse_case", data->lexer);

    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    if ((rc = parse_case_clause(case_node->case_item_list, data))
        == PARSER_ERROR)
        return PARSER_ERROR;

    if (!lexer_consume(data->lexer, TOKEN_ESAC))
        return syntax_error("parse_case", data->lexer);

    return PARSER_DONE;
}

void ast_free_case(struct ast_node *node)
{
    struct ast_node_case *case_node = to_node(node);

    free(case_node->var);
    list_free_elements(case_node->case_item_list, free_case_item);
    list_destroy(case_node->case_item_list);
}

void ast_print_case(FILE *file, struct ast_node *node)
{
    struct ast_node_case *case_node = to_node(node);
    ast_print_end_node(file, node, NULL);

    struct list_iterator *iterator = iterator_init(case_node->case_item_list);
    struct case_item *case_item = NULL;
    while ((case_item = iterator_next(iterator)))
    {
        // CASE -> CASE_ITEM
        ast_print_start_node(file, node, NULL);
        print_case_item(file, case_item);
        fprintf(file, "\n");

        // CASE_ITEM -> CASE_ITEM_BODY
        print_case_item(file, case_item);
        fprintf(file, " -> ");
        ast_print(file, case_item->body);
    }
    iterator_destroy(iterator);
}
