#include "parser.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_printer.h"
#include "lexer.h"
#include "token.h"

void *ast_node_alloc(enum ast_node_type type)
{
    static size_t type_sizes[] = {
        [NODE_OPERATOR] = sizeof(struct ast_node_operator),
        // -----
        [NODE_INPUT] = sizeof(struct ast_node_input),
        [NODE_PIPELINE] = sizeof(struct ast_node_pipeline),
        [NODE_COMMAND] = sizeof(struct ast_node_command),
        [NODE_SIMPLE_COMMAND] = sizeof(struct ast_node_simple_command),
        [NODE_FOR] = sizeof(struct ast_node_for),
        [NODE_WHILE] = sizeof(struct ast_node_while),
        [NODE_UNTIL] = sizeof(struct ast_node_until),
        [NODE_IF] = sizeof(struct ast_node_if),
        [NODE_CASE] = sizeof(struct ast_node_case),
    };
    static size_t last_node_id = 0;

    if (!type_sizes[type])
        errx(1, "No size found for type: '%s'", ast_type_to_str(type));

    struct ast_node *node = calloc(1, type_sizes[type]);
    node->type = type;
    node->id = last_node_id++;
    return node;
}

void *ast_node_alloc_attach(enum ast_node_type type, struct ast_node **result)
{
    *result = ast_node_alloc(type);
    return *result;
}

void ast_node_free(void *_node)
{
    if (_node == NULL)
        return;

    typedef void (*ast_free_f)(struct ast_node *);
    ast_free_f ast_free_func[] = {
        [NODE_OPERATOR] = ast_free_operator,
        // -----
        [NODE_INPUT] = ast_free_input,
        [NODE_PIPELINE] = ast_free_pipeline,
        [NODE_COMMAND] = ast_free_command,
        [NODE_SIMPLE_COMMAND] = ast_free_simple_command,
        [NODE_IF] = ast_free_if,
        [NODE_WHILE] = ast_free_while,
        [NODE_UNTIL] = ast_free_until,
        [NODE_FOR] = ast_free_for,
        [NODE_CASE] = ast_free_case,
    };

    struct ast_node *node = _node;

    if (!ast_free_func[node->type])
        errx(1, "No free function found for type: '%s'",
             ast_type_to_str(node->type));

    ast_free_func[node->type](node);
    free(node);
}

enum parser_status syntax_error(const char *function_name, struct lexer *lexer)
{
    print_syntax_error(function_name, lexer);
    return PARSER_ERROR;
}

enum parser_status not_done_error(int rc, const char *function_name,
                                  struct lexer *lexer)
{
    if (rc == PARSER_NO_MATCH)
        print_syntax_error(function_name, lexer);
    return PARSER_ERROR;
}
