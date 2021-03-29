#ifndef PARSER_H_197321
#define PARSER_H_197321

#include <stdbool.h>

#include "ast_a.h"
#include "lexer_a.h"

/**
** \brief Parse from the given lexer and allocate a new ast in `*ast`
**
** \return true if no error occured, false otherwise.
** \param lexer lexer to get token from
** \param ast placeholder for the ast to build
*/
bool parse_input_a(struct lexer *lexer, struct ast **ast);

bool parse_parenthesis(struct lexer *lexer, struct ast **ast);

bool parse_unit(struct lexer *lexer, struct ast **ast);

bool parse_mul(struct lexer *lexer, struct ast **ast);

bool parse_expo(struct lexer *lexer, struct ast **ast);

bool parse_add(struct lexer *lexer, struct ast **ast);

bool parse_and(struct lexer *lexer, struct ast **ast);

bool parse_xor(struct lexer *lexer, struct ast **ast);

bool parse_or(struct lexer *lexer, struct ast **ast);

bool parse_logic_and(struct lexer *lexer, struct ast **ast);

bool parse_logic_or(struct lexer *lexer, struct ast **ast);

bool parse_negative(struct lexer *lexer, struct ast **ast);

bool parse_positive(struct lexer *lexer, struct ast **ast);

bool parse_bitwise_not(struct lexer *lexer, struct ast **ast);

#endif /* ! PARSER_H_197321 */
