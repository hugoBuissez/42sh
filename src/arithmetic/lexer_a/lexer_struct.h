#ifndef LEXER_STRUCT_H
#define LEXER_STRUCT_H

struct lexer
{
    struct token *token;
    struct lexer *next;
    // struct token *current;
    int consumed;
};

#endif
