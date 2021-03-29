#include "eval.h"
#include "parser.h"

enum eval_status eval_pipeline(struct ast_node *node, struct argv_list *args,
                               struct sh_data *data)
{
    struct ast_node_pipeline *node_pipeline = to_node(node);

    int command_status = eval_node(node_pipeline->commmand, args, data);

    if (node_pipeline->negation)
    {
        data->rc = !command_status;
        return !command_status;
    }

    data->rc = command_status;
    return command_status;
}
