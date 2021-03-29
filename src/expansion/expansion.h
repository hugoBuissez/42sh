#ifndef EXPANSIONS_H
#define EXPANSIONS_H

#include "dynamic_string.h"
#include "shell.h"

struct expansion_data
{
    char *input;
    size_t exp_len;
    bool force_split;
    struct argv_list *args;
    struct sh_data *data;
    bool in_quotes;
    char *IFS;
};

char *eval_command_sub(char *input, struct sh_data *data);
char *eval_arithmetic_expr(char *input);

char *expand_bracket_param(struct expansion_data *exp_data);
char *expand_param(struct expansion_data *exp_data);
char *expand_tilde(struct expansion_data *exp_data);
char *expand_command(struct expansion_data *exp_data);
char *expand_arithmetic(struct expansion_data *exp_data);
char *expand(struct expansion_data *exp_data);

char *expand_word_no_split(char *word, struct argv_list *args,
                           struct sh_data *data);

void field_splitting(char *field, struct argv_list *words,
                     struct d_string *buffer, char *IFS);

struct argv_list *expand_pathname(char *word, struct sh_data *data);

void quote_removal(char **word);

struct argv_list *get_expanded_words(struct argv_list *words,
                                     struct argv_list *args,
                                     struct sh_data *data);

#endif /* end of include guard: EXPANSIONS_H */
