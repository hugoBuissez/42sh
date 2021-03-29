#include "eval.h"
#include "parser.h"

enum eval_status eval_input(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data)
{
    struct ast_node_input *node_input = to_node(node);

    return eval_node(node_input->list, args, data);
}
