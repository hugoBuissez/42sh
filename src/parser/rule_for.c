#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "ast_printer.h"
#include "list.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_for(struct ast_node **result, struct sh_data *data)
{
    int rc;

    if (!lexer_consume(data->lexer, TOKEN_FOR))
        return PARSER_NO_MATCH;

    struct ast_node_for *for_node = ast_node_alloc_attach(NODE_FOR, result);
    for_node->elements = argv_list_init();

    if (!(lexer_consume_word(data->lexer, TOKEN_WORD, &(for_node->var))))
        return syntax_error("parse_for", data->lexer);

    if (!lexer_consume(data->lexer, TOKEN_SEMICOLON))
    {
        while (lexer_consume(data->lexer, TOKEN_NEWLINE))
            continue;
        if (!lexer_consume_specific_word(data->lexer, "in"))
            return syntax_error("parse_for", data->lexer);

        char *word = NULL;
        while (lexer_consume_any_word(data->lexer, &word))
        {
            argv_list_append(for_node->elements, word);
            free(word);
        }

        if (!lexer_consume(data->lexer, TOKEN_SEMICOLON)
            && !lexer_consume(data->lexer, TOKEN_NEWLINE))
            return syntax_error("parse_for", data->lexer);
    }

    while (lexer_consume(data->lexer, TOKEN_NEWLINE))
        continue;

    if ((rc = parse_do_group(&(for_node->for_body), data)) != PARSER_DONE)
        return not_done_error(rc, "parse_for", data->lexer);

    return PARSER_DONE;
}

void ast_free_for(struct ast_node *node)
{
    struct ast_node_for *for_node = to_node(node);

    free(for_node->var);
    argv_list_free(for_node->elements);
    ast_node_free(for_node->for_body);
}

void ast_print_for(FILE *file, struct ast_node *node)
{
    struct ast_node_for *for_node = to_node(node);
    ast_print_end_node(file, node, NULL);

    ast_print_start_node(file, node, NULL);
    fprintf(file, "\"%s in ", for_node->var);

    struct argv_list *elements = for_node->elements;
    for (size_t i = 0; i < elements->len; ++i)
        fprintf(file, "%s ", elements->argv[i]);

    fprintf(file, "\"\n");

    ast_print_start_node(file, node, NULL);
    ast_print(file, for_node->for_body);
}
