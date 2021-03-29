#include "ast_printer.h"
#include "parser.h"
#include "token.h"

enum parser_status parse_pipeline(struct ast_node **result,
                                  struct sh_data *data)
{
    int rc;
    bool negation = lexer_consume(data->lexer, TOKEN_NOT);
    struct ast_node *command_node = NULL;

    if ((rc = parse_command(&command_node, data)) != PARSER_DONE)
        return rc;

    struct ast_node_pipeline *pipeline_node =
        ast_node_alloc_attach(NODE_PIPELINE, result);

    pipeline_node->negation = negation;
    pipeline_node->commmand = command_node;

    while (lexer_consume(data->lexer, TOKEN_PIPE))
    {
        while (lexer_consume(data->lexer, TOKEN_NEWLINE))
            continue;

        if ((rc = parse_command(&command_node, data)) != PARSER_DONE)
            return not_done_error(rc, "parse_pipeline", data->lexer);

        struct ast_node_operator *op = ast_node_alloc(NODE_OPERATOR);
        op->operator_type = OPE_PIPE;
        op->left = pipeline_node->commmand;
        op->right = command_node;
        command_node = NULL;

        pipeline_node->commmand = to_node(op);
    }

    return PARSER_DONE;
}

void ast_free_pipeline(struct ast_node *node)
{
    struct ast_node_pipeline *pipeline_node = to_node(node);
    ast_node_free(pipeline_node->commmand);
}

void ast_print_pipeline(FILE *file, struct ast_node *node)
{
    struct ast_node_pipeline *pipeline_node = to_node(node);
    ast_print(file, pipeline_node->commmand);
}
