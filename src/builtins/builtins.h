#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdbool.h>

#include "argv_list.h"
#include "shell.h"

bool is_builtin(char *cmd);

int builtin_echo(struct argv_list *args, struct sh_data *data);
int builtin_cd(struct argv_list *args, struct sh_data *data);
int builtin_exit(struct argv_list *args, struct sh_data *data);
int builtin_export(struct argv_list *args, struct sh_data *data);
int builtin_source(struct argv_list *args, struct sh_data *data);
int builtin_alias(struct argv_list *args, struct sh_data *data);
int builtin_unalias(struct argv_list *args, struct sh_data *data);

int exec_builtin(struct argv_list *args, struct sh_data *data);

#endif /* ! BUILTINS_H */
