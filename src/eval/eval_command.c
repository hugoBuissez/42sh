#include "eval.h"
#include "redirections.h"

enum eval_status eval_command(struct ast_node *node, struct argv_list *args,
                              struct sh_data *data)
{
    struct ast_node_command *node_command = to_node(node);

    struct list *redirect_operations =
        apply_redirections(node_command->redirections);

    if (redirect_operations == NULL) // Failed to apply redirections
        return EVAL_ERROR;

    enum eval_status exit_code = eval_node(node_command->command, args, data);

    restore_redirections(redirect_operations);

    return exit_code;
}
