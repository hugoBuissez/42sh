#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "argv_list.h"
#include "list.h"

enum ast_node_type
{
    NODE_OPERATOR,
    // -----
    NODE_INPUT,
    NODE_PIPELINE,
    NODE_COMMAND,
    NODE_SIMPLE_COMMAND,
    NODE_FOR,
    NODE_WHILE,
    NODE_UNTIL,
    NODE_IF,
    NODE_CASE,
};

enum operator_type
{
    OPE_SEPARATOR,
    OPE_AND,
    OPE_OR,
    OPE_PIPE,
};

// ##### NODE BASES #####

struct ast_node
{
    enum ast_node_type type;
    size_t id;
};

struct ast_node_operator
{
    struct ast_node base;
    enum operator_type operator_type;
    struct ast_node *left;
    struct ast_node *right;
};

// ##### NODES #####

struct ast_node_input
{
    struct ast_node base;
    struct ast_node *list;
};

struct ast_node_pipeline
{
    struct ast_node base;
    bool negation;
    struct ast_node *commmand;
};

struct ast_node_command
{
    struct ast_node base;
    struct ast_node *command;
    struct list *redirections;
};

struct ast_node_simple_command
{
    struct ast_node base;
    struct argv_list *argument_list;
    struct argv_list *assignments;
    struct list *redirections;
};

struct ast_node_for
{
    struct ast_node base;
    char *var;
    struct argv_list *elements;
    struct ast_node *for_body;
};

struct ast_node_while
{
    struct ast_node base;
    struct ast_node *condition;
    struct ast_node *while_body;
};

struct ast_node_until
{
    struct ast_node base;
    struct ast_node *condition;
    struct ast_node *until_body;
};

struct ast_node_case
{
    struct ast_node base;
    char *var;
    struct list *case_item_list;
};

struct ast_node_if
{
    struct ast_node base;
    struct ast_node *condition;
    struct ast_node *if_body;
    struct ast_node *else_body;
};

// ##### NODE FREE FUNCTIONS #####

void ast_free_operator(struct ast_node *node);
void ast_free_input(struct ast_node *node);
void ast_free_pipeline(struct ast_node *node);
void ast_free_command(struct ast_node *node);
void ast_free_simple_command(struct ast_node *node);
void ast_free_compound_list(struct ast_node *node);
void ast_free_if(struct ast_node *node);
void ast_free_while(struct ast_node *node);
void ast_free_for(struct ast_node *node);
void ast_free_until(struct ast_node *node);
void ast_free_do_group(struct ast_node *node);
void ast_free_case(struct ast_node *node);

// ##### GENERIC FUNCTIONS #####

void *ast_node_alloc(enum ast_node_type type);
void *ast_node_alloc_attach(enum ast_node_type type, struct ast_node **result);
void ast_node_free(void *_node);

// ##### UTILS #####

static inline void *to_node(void *node)
{
    return node;
}

#endif /* ! AST_H */
