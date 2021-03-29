#include "expansion.h"

#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"
#include "dynamic_string.h"
#include "expansion.h"
#include "lexer.h"
#include "lexer_internals.h"
#include "lexer_rules.h"
#include "string_utils.h"
#include "variables.h"

struct expansion_data init_exp_data(char *input, struct argv_list *args,
                                    struct sh_data *data)
{
    struct expansion_data exp_data;

    exp_data.input = input;
    exp_data.exp_len = 0;
    exp_data.force_split = false;
    exp_data.args = args;
    exp_data.data = data;
    exp_data.in_quotes = false;
    exp_data.force_split = false;
    exp_data.IFS = find_sh_variable("IFS", data);

    return exp_data;
}

static struct lexer *setup_expansion_lexer(char *input, struct d_string *buffer)
{
    struct lexer *lexer = calloc(1, sizeof(*lexer));
    if (lexer == NULL)
        errx(1, "Memory exhausted");

    lexer->line = input;
    lexer->ptr = input;
    lexer->state = IN_WORD;
    lexer->stream_in = NULL;
    lexer->curr_tok = buffer;

    return lexer;
}

static void apply_expansion(struct lexer *lexer, struct argv_list *words,
                            struct expansion_data *exp_data)
{
    struct d_string *buffer = lexer->curr_tok;
    lexer->curr_tok = NULL;

    exp_data->input = (lexer->ptr - 1); // Mark start of expansion
    rule_5(lexer, BUFFERISING);
    exp_data->exp_len = lexer->ptr - exp_data->input; // Retrieve len of exp

    char *res = expand(exp_data);
    if (res)
    {
        if (words != NULL && (!exp_data->in_quotes || exp_data->force_split))
            field_splitting(res, words, buffer, exp_data->IFS);
        else
            string_append(buffer, res);
    }
    free(res);

    lexer->ptr = exp_data->input + exp_data->exp_len;
    lexer->curr_tok = buffer; // Restore buffer
}

static void apply_tilde_expansion(struct lexer *lexer,
                                  struct expansion_data *exp_data)
{
    char *tilde_exp = NULL;
    if ((tilde_exp = expand_tilde(exp_data)))
    {
        string_append(lexer->curr_tok, tilde_exp);
        free(tilde_exp);
        lexer->ptr = exp_data->input + exp_data->exp_len;
    }
}

char *expand_word_split(char *word, struct argv_list *words,
                        struct argv_list *args, struct sh_data *data)
{
    struct d_string *buffer = string_create("");
    struct lexer *lexer = setup_expansion_lexer(word, buffer);
    struct expansion_data exp_data = init_exp_data(word, args, data);

    apply_tilde_expansion(lexer, &exp_data); // Can only happen at start

    while (next_sym(lexer))
    {
        if (lexer->sym == EOF)
            break;

        if (lexer->sym == '\"' && !is_escaped(lexer))
            exp_data.in_quotes = !exp_data.in_quotes;

        if (lexer->sym == '\'' && !is_escaped(lexer) && !exp_data.in_quotes)
            rule_4(lexer, BUFFERISING);

        else if (lexer->sym == '\\' && !is_escaped(lexer))
            rule_4(lexer, BUFFERISING);

        else if (is_expansion(lexer->sym) && !is_escaped(lexer))
            apply_expansion(lexer, words, &exp_data);

        else
            append_curr(lexer);
    }

    if (words != NULL && string_len(buffer) > 0) // Append leftover
        argv_list_append(words, string_value(buffer));

    char *non_split_res = NULL;
    if (words == NULL) // We want the raw result of expansions
        non_split_res = string_value_copy(buffer);

    free(lexer);
    string_destroy(buffer);

    return non_split_res;
}

char *expand_word_no_split(char *word, struct argv_list *args,
                           struct sh_data *data)
{
    char *exp = expand_word_split(word, NULL, args, data);
    quote_removal(&exp);
    return exp;
}

struct argv_list *get_expanded_words(struct argv_list *words,
                                     struct argv_list *args,
                                     struct sh_data *data)
{
    struct argv_list *exp_words = argv_list_init();
    struct argv_list *exp_paths = argv_list_init();

    // Expansions
    for (size_t i = 0; words->argv[i] != NULL; i++)
        expand_word_split(words->argv[i], exp_words, args, data);

    // Pathname expansion
    for (size_t i = 0; exp_words->argv[i] != NULL; i++)
    {
        struct argv_list *word_paths =
            expand_pathname(exp_words->argv[i], data);

        if (!word_paths || word_paths->argv[0] == NULL)
        {
            if (!(word_paths && word_paths->argv[0] == NULL
                  && data->options->nullglob))
                argv_list_append(exp_paths, exp_words->argv[i]);
            argv_list_free(word_paths);
            continue;
        }

        for (size_t j = 0; word_paths->argv[j] != NULL; j++)
        {
            if (*(word_paths->argv[j]) != 0)
                argv_list_append(exp_paths, word_paths->argv[j]);
        }
        argv_list_free(word_paths);
    }

    argv_list_free(exp_words);

    // Quote removal
    for (size_t i = 0; exp_paths->argv[i] != NULL; i++)
        quote_removal(exp_paths->argv + i);

    return exp_paths;
}
