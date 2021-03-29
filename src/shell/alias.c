#include "alias.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"
#include "lexer.h"
#include "list.h"

static void get_alias_tokens(struct alias *alias)
{
    if (!alias || !alias->raw_value)
        return;

    size_t input_len = strlen(alias->raw_value);
    if (input_len == 0)
        return;

    FILE *stream = fmemopen(alias->raw_value, input_len, "r");

    struct lexer *lexer = lexer_init(stream);

    struct token *token = NULL;
    while ((token = lexer_pop(lexer)))
    {
        if (token->type == TOKEN_EOF)
        {
            token_destroy(token);
            break;
        }
        list_append(alias->tokens, token);
    }

    lexer_destroy(lexer);
}

static struct alias *create_alias(char *name, char *value)
{
    struct alias *alias = calloc(1, sizeof(*alias));
    if (!alias)
        errx(1, "Memory exhausted");

    alias->name = strdup(name);
    alias->raw_value = strdup(value);
    alias->tokens = list_init();

    get_alias_tokens(alias);

    return alias;
}

void add_alias(char *name, char *value, struct hash_set *alias_table)
{
    struct alias *alias = create_alias(name, value);

    struct hash_slot *slot = NULL;
    if ((slot = hash_find_slot(alias_table, name)) != NULL)
    {
        free_alias(slot->data);
        slot->data = alias;
    }
    else
        hash_insert(alias_table, strdup(name), alias);
}

void free_alias(void *_alias)
{
    struct alias *alias = _alias;

    free(alias->name);
    free(alias->raw_value);
    list_free_elements(alias->tokens, token_destroy);
    list_destroy(alias->tokens);
    free(alias);
}
