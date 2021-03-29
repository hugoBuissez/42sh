#include "lexer_internals.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "alias.h"
#include "token.h"

bool is_valid_operator(char *word)
{
    static char *operator[] = {
        "&&", "||", ";;", "&",  ";",  "|",  "(", ")", "<<-",
        "<<", ">>", ">|", "<&", ">&", "<>", "<", ">",
    };
    for (size_t i = 0; i < sizeof(operator) / sizeof(operator[0]); i++)
        if (strcmp(word, operator[i]) == 0)
            return true;

    return false;
}

int get_new_line(struct lexer *lexer)
{
    char *new_line = NULL;
    size_t n = 0;
    ssize_t bytes_read = -1;
    if (lexer->stream_in)
        bytes_read = getline(&new_line, &n, lexer->stream_in);

    if (bytes_read == -1) // EOF
    {
        lexer->ptr = NULL;
        free(new_line);
        return EOF;
    }

    free(lexer->line);
    lexer->line = new_line;
    lexer->ptr = lexer->line;

    return 0;
}

void delim_token(struct lexer *lexer)
{
    if (string_len(lexer->curr_tok) == 0)
        return;

    char *token_value = string_value(lexer->curr_tok);

    // If first first element of input, check if alias exists
    struct alias *alias = NULL;
    if (list_empty(lexer->token_list)
        && (alias = hash_find(lexer->alias_table, token_value)))
    {
        struct list_iterator *iter = iterator_init(alias->tokens);
        struct token *token = NULL;
        while ((token = iterator_next(iter)))
        {
            list_append(lexer->token_list,
                        token_create(token->type, token->value));
        }
        iterator_destroy(iter);
    }
    else
    {
        struct token *token = create_token_from_word(token_value, lexer->sym);
        list_append(lexer->token_list, token);
    }

    string_empty(lexer->curr_tok);
}
