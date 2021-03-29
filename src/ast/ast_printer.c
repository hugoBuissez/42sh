#include "ast_printer.h"

#include <err.h>

#include "parser.h"
#include "token.h"

const char *ast_type_to_str(enum ast_node_type type)
{
    static const char *type_str[] = {
        [NODE_OPERATOR] = "OPERATOR",
        // -----
        [NODE_INPUT] = "INPUT",
        [NODE_PIPELINE] = "PIPELINE",
        [NODE_COMMAND] = "COMMAND",
        [NODE_SIMPLE_COMMAND] = "SIMPLE_COMMAND",
        [NODE_FOR] = "FOR",
        [NODE_WHILE] = "WHILE",
        [NODE_UNTIL] = "UNTIL",
        [NODE_IF] = "IF",
        [NODE_CASE] = "CASE",
    };

    if (!type_str[type])
        errx(1, "No type str found for type: '%s'", ast_type_to_str(type));

    return type_str[type];
}

void ast_print(FILE *file, struct ast_node *node)
{
    if (node == NULL)
        return;

    typedef void (*ast_free_f)(FILE * file, struct ast_node *);
    ast_free_f ast_print_func[] = {
        [NODE_OPERATOR] = ast_print_operator,
        // -----
        [NODE_INPUT] = ast_print_input,
        [NODE_PIPELINE] = ast_print_pipeline,
        [NODE_COMMAND] = ast_print_command,
        [NODE_SIMPLE_COMMAND] = ast_print_simple_command,
        [NODE_FOR] = ast_print_for,
        [NODE_WHILE] = ast_print_while,
        [NODE_UNTIL] = ast_print_until,
        [NODE_IF] = ast_print_if,
        [NODE_CASE] = ast_print_case,
    };

    if (!ast_print_func[node->type])
        errx(1, "No print function found for type: '%s'",
             ast_type_to_str(node->type));

    ast_print_func[node->type](file, node);
}
