#ifndef EXEC_H
#define EXEC_H

#include "argv_list.h"
#include "ast.h"
#include "exec.h"
#include "shell.h"

/**
 * @brief                      Executes the given command
 * @param arguments            Command arguments
 * @param data                 Shell data
 * @return                     Return code
 */
int exec_simple_command(struct argv_list *arguments, struct sh_data *data);

/**
 * @brief                      Executes one or several pipe command(s)
 * @param left                 Left node of the pipe
 * @param right                Right node of the pipe
 * @param data                 Shell data
 * @return                     Return code
 */
int exec_pipe(struct ast_node *left, struct ast_node *right,
              struct argv_list *args, struct sh_data *data);

/**
 * @brief                      Executes a builtin command
 * @param args                 The builtin argument(s) (including the command
 *                             itself)
 * @return                     Return code
 */
int exec_builtin(struct argv_list *args, struct sh_data *data);

#endif /* ! EXEC_H */
