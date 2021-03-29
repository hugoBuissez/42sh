#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>

#include "list.h"
#include "token.h"

struct lexer;

/**
 * @brief   Initialises a new lexer given a stream
 * @param   The lexer stream
 * @return  The new lexer
 */
struct lexer *lexer_init(FILE *);

/**
 * @brief        Close the stream of a given lexer
 * @param lexer  The lexer to close the stream from
 */
void lexer_close_stream(struct lexer *lexer);

/**
 * @brief        Frees the given lexer
 * @param lexer  The lexer to free
 */
void lexer_destroy(struct lexer *lexer);

/**
 * @brief        Returns the next token and removes it
 * @param lexer  The lexer
 * @return       The next token
 */
struct token *lexer_pop(struct lexer *lexer);

/**
 * @brief        Returns the next token without removing it
 * @param lexer  The lexer
 * @return       The next token
 */
struct token *lexer_peek(struct lexer *lexer);

/**
 * @brief        Pops next token if matching type
 * @param lexer  The lexer
 * @param type   The type to match
 * @return       Wether type was matched
 */
bool lexer_consume(struct lexer *lexer, enum token_type type);

/**
 * @brief           Pops and sets 'ret_word' if matching type
 * @param lexer     The lexer
 * @param type      The type to match
 * @param ret_word  The token value
 * @return          Wether type was matched
 */
bool lexer_consume_word(struct lexer *lexer, enum token_type type,
                        char **ret_word);

/**
 * @brief           Pops next token if WORD
 * @param lexer     The lexer
 * @param ret_word  The token value
 * @return          Wether type was matched
 */
bool lexer_consume_any_word(struct lexer *lexer, char **ret_word);

/**
 * @brief                Pops next token if matching word value
 * @param lexer          The lexer
 * @param specific_word  The token value
 * @return               Wether type was matched
 */
bool lexer_consume_specific_word(struct lexer *lexer, char *specific_word);

/**
 * @brief        Goes through the lexer and check for a next  function
 *               declaration
 * @param lexer  The lexer
 * @return       Wether next tokens constitutes function declaration
 */
bool lexer_is_funcdec(struct lexer *lexer);

void print_syntax_error(const char *function_name, struct lexer *lexer);

void print_peek(struct lexer *lexer);
void print_lexer(struct lexer *lexer);

#endif /* LEXER_H */
