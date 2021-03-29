#include "eval.h"

#include <err.h>
#include <stddef.h>

#include "ast.h"
#include "ast_printer.h"

enum eval_status eval_node(struct ast_node *node, struct argv_list *args,
                           struct sh_data *data)
{
    if (node == NULL)
        return EVAL_TRUE;

    typedef enum eval_status (*ast_eval_f)(
        struct ast_node *, struct argv_list *, struct sh_data *);

    ast_eval_f ast_eval_func[] = {
        [NODE_OPERATOR] = eval_operator,
        // -----
        [NODE_INPUT] = eval_input,
        [NODE_PIPELINE] = eval_pipeline,
        [NODE_COMMAND] = eval_command,
        [NODE_SIMPLE_COMMAND] = eval_simple_command,
        [NODE_FOR] = eval_for,
        [NODE_WHILE] = eval_while,
        [NODE_UNTIL] = eval_until,
        [NODE_CASE] = eval_case,
        [NODE_IF] = eval_if,
    };

    if (!ast_eval_func[node->type])
        errx(1, "No eval function found for type: '%s'",
             ast_type_to_str(node->type));

    return ast_eval_func[node->type](node, args, data);
}
