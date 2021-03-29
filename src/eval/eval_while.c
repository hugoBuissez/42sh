#include "eval.h"

enum eval_status eval_while(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data)
{
    struct ast_node_while *node_while = to_node(node);

    enum eval_status rc = EVAL_TRUE;
    while (rc != EVAL_BREAK
           && eval_node(node_while->condition, args, data) == EVAL_TRUE)
        rc = eval_node(node_while->while_body, args, data);

    return rc == EVAL_BREAK || rc == EVAL_CONTINUE ? EVAL_TRUE : rc;
}
