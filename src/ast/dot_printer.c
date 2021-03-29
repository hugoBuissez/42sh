#include <err.h>
#include <stdio.h>

#include "ast_printer.h"
#include "parser.h"
#include "shell.h"
#include "token.h"

void ast_print_start_node(FILE *file, struct ast_node *node, const char *value)
{
    if (!node)
        return;
    fprintf(file, "\"%lu: %s\" -> ", node->id,
            value ? value : ast_type_to_str(node->type));
}

void ast_print_end_node(FILE *file, struct ast_node *node, const char *value)
{
    if (!node)
        return;
    fprintf(file, "\"%lu: %s\"\n", node->id,
            value ? value : ast_type_to_str(node->type));
}

void init_dot_file(struct sh_data *data)
{
    FILE *file;
    if (!data->options->print_output_file
        || !(file = fopen(data->options->print_output_file, "w+")))
        return;

    fprintf(file, "digraph tree {\n");

    fclose(file);
}

void add_input_to_dot_file(struct sh_data *data, struct ast_node *input)
{
    FILE *file;
    if (!data->options->print_output_file
        || !(file = fopen(data->options->print_output_file, "a")))
        return;

    ast_print(file, input);

    fclose(file);
}

void end_dot_file(struct sh_data *data)
{
    FILE *file;
    if (!data->options->print_output_file
        || !(file = fopen(data->options->print_output_file, "a")))
        return;

    fprintf(file, "}\n");

    fclose(file);
}
