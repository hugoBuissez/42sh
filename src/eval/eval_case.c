#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"
#include "eval.h"
#include "expansion.h"
#include "list.h"

static bool case_item_matches(char *variable, struct case_item *case_item,
                              struct argv_list *args, struct sh_data *data)
{
    struct argv_list *elements = case_item->elements;

    for (size_t i = 0; i < elements->len; i++)
    {
        char *element = expand_word_no_split(elements->argv[i], args, data);

        if (strcmp(variable, element) == 0)
        {
            free(element);
            return true;
        }

        free(element);
    }
    return false;
}

enum eval_status eval_case(struct ast_node *node, struct argv_list *args,
                           struct sh_data *data)
{
    struct ast_node_case *node_case = to_node(node);

    struct list_iterator *iter = iterator_init(node_case->case_item_list);

    struct case_item *case_item = NULL;
    char *exp_var = expand_word_no_split(node_case->var, args, data);

    while ((case_item = iterator_next(iter)))
    {
        if (case_item_matches(exp_var, case_item, args, data))
        {
            free(exp_var);
            iterator_destroy(iter);
            return eval_node(case_item->body, args, data);
        }
    }

    free(exp_var);
    iterator_destroy(iter);

    return EVAL_TRUE;
}
