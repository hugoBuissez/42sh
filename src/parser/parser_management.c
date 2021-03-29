#include "ast_printer.h"
#include "parser.h"

void ast_free_operator(struct ast_node *node)
{
    struct ast_node_operator *operator_node = to_node(node);
    ast_node_free(operator_node->left);
    ast_node_free(operator_node->right);
}

void ast_print_operator(FILE *file, struct ast_node *node)
{
    struct ast_node_operator *operator_node = to_node(node);

    static const char *operator_str[] = {
        [OPE_SEPARATOR] = "SEP",
        [OPE_AND] = "&&",
        [OPE_OR] = "||",
        [OPE_PIPE] = "|",
    };
    const char *ope_str = operator_str[operator_node->operator_type];

    ast_print_end_node(file, node, ope_str);

    ast_print_start_node(file, node, ope_str);
    ast_print(file, operator_node->left);

    ast_print_start_node(file, node, ope_str);
    ast_print(file, operator_node->right);
}
