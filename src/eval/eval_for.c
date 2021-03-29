#include "eval.h"
#include "expansion.h"
#include "list.h"
#include "variables.h"

enum eval_status eval_for(struct ast_node *node, struct argv_list *args,
                          struct sh_data *data)
{
    struct ast_node_for *node_for = to_node(node);
    struct argv_list *elements =
        get_expanded_words(node_for->elements, args, data);

    enum eval_status rc = EVAL_TRUE;
    for (size_t i = 0; i < elements->len && rc != EVAL_BREAK; i++)
    {
        set_sh_variable(node_for->var, elements->argv[i], false, data);
        rc = eval_node(node_for->for_body, args, data);
    }

    argv_list_free(elements);

    return rc == EVAL_BREAK || rc == EVAL_CONTINUE ? EVAL_TRUE : rc;
}
