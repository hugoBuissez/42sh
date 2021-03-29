#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "ast.h"
#include "parser.h"

void ast_print_operator(FILE *file, struct ast_node *node);

void ast_print_input(FILE *file, struct ast_node *node);
void ast_print_pipeline(FILE *file, struct ast_node *node);
void ast_print_command(FILE *file, struct ast_node *node);
void ast_print_simple_command(FILE *file, struct ast_node *node);
void ast_print_for(FILE *file, struct ast_node *node);
void ast_print_while(FILE *file, struct ast_node *node);
void ast_print_until(FILE *file, struct ast_node *node);
void ast_print_if(FILE *file, struct ast_node *node);
void ast_print_case(FILE *file, struct ast_node *node);
void ast_print_case_item(FILE *file, struct ast_node *node);

void print_redirection(FILE *file, struct redirection *redirection);
void print_case_item(FILE *file, struct case_item *case_item);

void ast_print_start_node(FILE *file, struct ast_node *node, const char *value);
void ast_print_end_node(FILE *file, struct ast_node *node, const char *value);
const char *ast_type_to_str(enum ast_node_type type);
void ast_print(FILE *file, struct ast_node *node);

void init_dot_file(struct sh_data *data);
void add_input_to_dot_file(struct sh_data *data, struct ast_node *input);
void end_dot_file(struct sh_data *data);

#endif /* AST_PRINTER_H */
