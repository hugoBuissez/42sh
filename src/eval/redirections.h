#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

#include "list.h"

/**
 * @brief                      Executes a redirection
 * @param redirections         The list of redirection(s) to execute
 * @return                     The list of executed redirection(s)
 */
struct list *apply_redirections(struct list *redirections);

/**
 * @brief                      Restores and free previous redirection(s)
 * @param redirect_operations  The previous executed redirection(s)
 */
void restore_redirections(struct list *redirect_operations);

#endif /* ! REDIRECTIONS_H */
