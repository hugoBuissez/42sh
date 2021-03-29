#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

enum token_type
{
    // ---- RESERVED WORDS
    TOKEN_NOT,
    TOKEN_L_BR,
    TOKEN_R_BR,
    TOKEN_CASE,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_ESAC,
    TOKEN_FI,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_IN,
    TOKEN_THEN,
    TOKEN_UNTIL,
    TOKEN_WHILE,
    // ----- WORDS
    TOKEN_ASSIGNMENT_WORD,
    TOKEN_WORD,
    // ----- OPERATORS
    TOKEN_AND_IF, // &&
    TOKEN_OR_IF, // ||
    TOKEN_DSEMI, // ;;
    TOKEN_AND, // &
    TOKEN_SEMICOLON, // ;
    TOKEN_PIPE, // |
    TOKEN_L_PAR, // (
    TOKEN_R_PAR, // )
    TOKEN_IO_NUMBER,
    TOKEN_REDIRECT, // all redirect operators
                    // ----- SPECIALS
    TOKEN_NEWLINE, // '\n'
    TOKEN_EOF,
};

struct token
{
    enum token_type type;
    char *value;
};

bool token_type_is_word(enum token_type type);

struct token *token_create(enum token_type type, char *value);
void token_destroy(void *_token);

const char *token_type_to_str(enum token_type type);
void print_token(struct token *token);

#endif /* TOKEN_H */
