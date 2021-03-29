#ifndef SHELL_H
#define SHELL_H

#include "htab.h"
#include "lexer.h"
#include "options.h"

struct sh_data
{
    struct options *options;
    struct lexer *lexer;
    struct ast_node *current_input;
    struct hash_set *variable_table;
    struct hash_set *function_table;
    struct hash_set *alias_table;
    int rc;
};

void setup_sub_shell(struct sh_data *data, struct sh_data *parent);
void setup_shell(struct sh_data *data);
int run_shell(struct sh_data *data);
void end_sub_shell(struct sh_data *data);
void end_shell(struct sh_data *data);

#endif /* ! SHELL_H */
