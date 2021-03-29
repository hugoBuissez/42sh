#include "eval.h"

enum eval_status eval_if(struct ast_node *node, struct argv_list *args,
                         struct sh_data *data)
{
    struct ast_node_if *node_if = to_node(node);

    enum eval_status eval_status;

    if (eval_node(node_if->condition, args, data) == EVAL_TRUE)
    {
        if ((eval_status = eval_node(node_if->if_body, args, data))
            != EVAL_TRUE)
            return eval_status;
    }
    else if (node_if->else_body != NULL)
        return eval_node(node_if->else_body, args, data);

    return EVAL_TRUE;
}
