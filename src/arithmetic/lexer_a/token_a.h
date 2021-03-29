#ifndef TOKEN_H_39453
#define TOKEN_H_39453

/**
** \brief Type of a token (operators, value, ...)
*/
enum token_type
{
    TOKEN_PLUS = 0,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_POWER,
    TOKEN_LOGIC_AND,
    TOKEN_LOGIC_OR,
    TOKEN_XOR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_BITWISE_NOT,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_EOF,
    TOKEN_INVALID,
    TOKEN_NUMBER
};

/**
** \brief Token struct declaration
*/
struct token
{
    /* type of the token */
    enum token_type type;
    /* hold the number (only used when type == TOKEN_NUMBER) */
    int value;
};

/**
** \brief String representation of a token (used for debugging and errors)
**
** \return a string describing the given token
** \param token to stringify
*/
const char *token_str(struct token *token);

/**
** \brief Token allocator and initialiser
**
** \return a pointer to the allocated token
*/
struct token *token_alloc(void);

/**
** \brief Wrapper to release memory of a token
**
** \param token the token to free
*/
void token_free(struct token *token);

#endif /* ! TOKEN_H_39453 */
