#include "eval.h"
#include "exec.h"
#include "parser.h"

enum eval_status eval_operator(struct ast_node *node, struct argv_list *args,
                               struct sh_data *data)
{
    struct ast_node_operator *node_operator = to_node(node);

    enum eval_status rc;
    switch (node_operator->operator_type)
    {
    case OPE_SEPARATOR:
        rc = eval_node(node_operator->left, args, data);
        if (rc == EVAL_BREAK || rc == EVAL_CONTINUE)
            return rc;
        return eval_node(node_operator->right, args, data);

    case OPE_AND:
        if ((rc = eval_node(node_operator->left, args, data)) != EVAL_TRUE)
            return rc;
        return eval_node(node_operator->right, args, data);

    case OPE_OR:
        rc = eval_node(node_operator->left, args, data);
        if (rc == EVAL_BREAK || rc == EVAL_CONTINUE)
            return rc;
        if (rc != EVAL_TRUE)
            return eval_node(node_operator->right, args, data);
        return rc;

    case OPE_PIPE:
        return exec_pipe(node_operator->left, node_operator->right, args, data);
    }

    return EVAL_ERROR;
}
