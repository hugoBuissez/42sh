#ifndef LEXER_INTERNALS_H
#define LEXER_INTERNALS_H

#include <ctype.h>
#include <fnmatch.h>
#include <stdbool.h>

#include "dynamic_string.h"
#include "htab.h"
#include "lexer.h"
#include "string_utils.h"

enum lexer_state
{
    UNKNOWN = -1,
    IN_WORD,
    IN_OPERATOR,
    IN_WHITESPACE,
};

enum reading_mode
{
    TOKENISING,
    BUFFERISING,
};

struct lexer
{
    struct list *token_list;
    struct hash_set *alias_table;
    // -----
    FILE *stream_in;
    char *line;
    char *ptr;
    // -----
    char sym;
    char *last_escaped;
    enum lexer_state state;
    struct d_string *curr_tok;
};

static inline bool is_escaped(struct lexer *lexer)
{
    return (lexer->ptr - 1) == lexer->last_escaped;
}

static inline bool first_char_of_op(int c)
{
    return c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<'
        || c == '>';
}

static inline int is_redirect_prefix(int c)
{
    return c == '<' || c == '>';
}

static inline int is_quoting(int c)
{
    return c == '\\' || c == '\'' || c == '\"';
}

static inline int is_expansion(int c)
{
    return c == '$' || c == '`';
}

static inline int valid_name_start(int c)
{
    return c == '_' || isalpha(c);
}

static inline int valid_name_char(int c)
{
    return c == '_' || isalnum(c);
}

// Lexer

static inline char peek_next_sym(struct lexer *lexer)
{
    return lexer->ptr ? *(lexer->ptr) : '\0';
}

static inline void append_curr(struct lexer *lexer)
{
    string_append_char(lexer->curr_tok, lexer->sym);
}

bool is_valid_operator(char *word);

int get_new_line(struct lexer *lexer);
void delim_token(struct lexer *lexer);

char next_sym(struct lexer *lexer);
void prev_sym(struct lexer *lexer);
struct token *create_token_from_word(char *word, char delim);

int lexer_get_tokens(struct lexer *lexer);

#endif /* ! LEXER_INTERNALS_H */
