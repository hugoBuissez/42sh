#include "eval.h"

enum eval_status eval_until(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data)
{
    struct ast_node_until *node_until = to_node(node);

    enum eval_status rc = EVAL_TRUE;
    while (rc != EVAL_BREAK
           && eval_node(node_until->condition, args, data) != EVAL_TRUE)
        rc = eval_node(node_until->until_body, args, data);

    return rc == EVAL_BREAK || rc == EVAL_CONTINUE ? EVAL_TRUE : rc;
}
