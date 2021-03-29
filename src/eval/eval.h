#ifndef EVAL_H
#define EVAL_H

#include "argv_list.h"
#include "ast.h"
#include "parser.h"
#include "shell.h"

enum eval_status
{
    EVAL_TRUE,
    EVAL_ERROR,
    EVAL_CONTINUE = 10,
    EVAL_BREAK,
    EVAL_NOT_FOUND = 127,
};

enum eval_status eval_node(struct ast_node *node, struct argv_list *args,
                           struct sh_data *data);

enum eval_status eval_operator(struct ast_node *node, struct argv_list *args,
                               struct sh_data *data);
enum eval_status eval_input(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data);
enum eval_status eval_pipeline(struct ast_node *node, struct argv_list *args,
                               struct sh_data *data);
enum eval_status eval_command(struct ast_node *node, struct argv_list *args,
                              struct sh_data *data);
enum eval_status eval_simple_command(struct ast_node *node,
                                     struct argv_list *args,
                                     struct sh_data *data);
enum eval_status eval_for(struct ast_node *node, struct argv_list *args,
                          struct sh_data *data);
enum eval_status eval_while(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data);
enum eval_status eval_until(struct ast_node *node, struct argv_list *args,
                            struct sh_data *data);
enum eval_status eval_case(struct ast_node *node, struct argv_list *args,
                           struct sh_data *data);
enum eval_status eval_if(struct ast_node *node, struct argv_list *args,
                         struct sh_data *data);

enum eval_status eval_redirection(struct redirection *redirection,
                                  struct sh_data *data);
enum eval_status eval_case_item(struct case_item *case_item,
                                struct sh_data *data);

#endif /* EVAL_H */
