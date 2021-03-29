#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "argv_list.h"
#include "ast.h"
#include "list.h"
#include "shell.h"

enum parser_status
{
    PARSER_EOF,
    PARSER_DONE,
    PARSER_NO_MATCH,
    PARSER_ERROR,
};

struct redirection
{
    char *io_number;
    char *operator;
    char *word;
};

struct case_item
{
    struct argv_list *elements;
    struct ast_node *body;
};

// ##### PARSE FUNCTIONS #####

/**
 * @brief        Parse the input rule
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_input(struct ast_node **result, struct sh_data *data);

/**
 * @brief        list:  and_or ((';'|'&') and_or)* [';'|'&']
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_list(struct ast_node **result, struct sh_data *data);

/**
 * @brief        and_or:  pipeline (('&&'|'||') ('\n')* pipeline)*
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_and_or(struct ast_node **result, struct sh_data *data);

/**
 * @brief        pipeline:  ['!'] command ('|' ('\n')* command)*
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_pipeline(struct ast_node **result,
                                  struct sh_data *data);

/**
 * @brief        command:  simple_command
 *                       | shell_command  (redirection)*
 *                       | funcdec (redirection)*
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_command(struct ast_node **result,
                                 struct sh_data *data);

/**
 * @brief        simple_command:  (prefix)+
 *                              | (prefix)* (element)+
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_simple_command(struct ast_node **result,
                                        struct sh_data *data);

/**
 * @brief        shell_command:  '{' compound_list '}'
 *                             | '(' compound_list ')'
 *                             | rule_for
 *                             | rule_while
 *                             | rule_until
 *                             | rule_case
 *                             | rule_if
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_shell_command(struct ast_node **result,
                                       struct sh_data *data);

/**
 * @brief        funcdec:  ['function'] WORD '(' ')' ('\n')* shell_command
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_funcdec(char **func_name, struct ast_node **func_body,
                                 struct sh_data *data);

/**
 * @brief        redirection:  [IONUMBER] '>' WORD
 *                           | [IONUMBER] '<' WORD
 *                           | [IONUMBER] '>>' WORD
 *                           | [IONUMBER] '<<' HEREDOC
 *                           | [IONUMBER] '<<-' HEREDOC
 *                           | [IONUMBER] '>&' WORD
 *                           | [IONUMBER] '<&' WORD
 *                           | [IONUMBER] '>|' WORD
 *                           | [IONUMBER] '<>' WORD
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_redirection(struct redirection **result,
                                     struct sh_data *data);

/**
 * @brief        prefix:  ASSIGNMENT_WORD
 *                      | redirection;
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_prefix(struct argv_list *assignments,
                                struct list *redirection_list,
                                struct sh_data *data);

/**
 * @brief        prefix:  WORD
 *                      | redirection;
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_element(struct argv_list *argument_list,
                                 struct list *redirection_list,
                                 struct sh_data *data);

/**
 * @brief        compound_list:  ('\n')* and_or ((';'|'&'|'\n') ('\n')* and_or)*
 *                              [('&'|';'|'\n') ('\n')*]
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_compound_list(struct ast_node **result,
                                       struct sh_data *data);

/**
 * @brief        rule_for:  For WORD ([';']|[('\n')*
 *                          'in' (WORD)* (';'|'\n')]) ('\n')* do_group
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_for(struct ast_node **result, struct sh_data *data);

/**
 * @brief        rule_while:  While compound_list do_group
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_while(struct ast_node **result, struct sh_data *data);

/**
 * @brief        rule_until:  While compound_list do_group
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_until(struct ast_node **result, struct sh_data *data);

/**
 * @brief        rule_case:  Case WORD ('\n')* 'in' ('\n')* [case_clause] Esac
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_case(struct ast_node **result, struct sh_data *data);

/**
 * @brief        rule_if:  If compound_list Then compound_list [else_clause] Fi
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_if(struct ast_node **result, struct sh_data *data);

/**
 * @brief        else_clause:  Else compound_list
 *                           | Elif compound_list Then compound_list
 *                             [else_clause]
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_else_clause(struct ast_node **result,
                                     struct sh_data *data);

/**
 * @brief        do_group:  Do compound_list Done
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_do_group(struct ast_node **result,
                                  struct sh_data *data);

/**
 * @brief        case_clause:  case_item (';;' ('\n')* case_item)*
 *                               [;;] ('\n')*
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_case_clause(struct list *case_item_list,
                                     struct sh_data *data);

/**
 * @brief        case_item:  ['('] WORD ('|' WORD)* ')' ('\n')*
 *                           [ compound_list ]
 *
 * @param result Result node
 * @param data   Shell data
 *
 * @returns      Return code
 */
enum parser_status parse_case_item(struct case_item **result,
                                   struct sh_data *data);

void free_redirection(void *_redirection);
void free_case_item(void *_case_item);

enum parser_status syntax_error(const char *function_name, struct lexer *lexer);
enum parser_status not_done_error(int rc, const char *function_name,
                                  struct lexer *lexer);

#endif /* PARSER_H */
