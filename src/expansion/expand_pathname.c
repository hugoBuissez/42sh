#include <dirent.h>
#include <err.h>
#include <fnmatch.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "argv_list.h"
#include "dynamic_string.h"
#include "options.h"
#include "string_utils.h"
#include "variables.h"

struct expand_options
{
    bool nocaseglob;
    bool dotglob;
    bool extglob;
    int flags;
};

static struct expand_options exp_opts;

static void sort_args(struct argv_list *res)
{
    qsort(res->argv, res->len, sizeof(char *), comp_str);
}

static void append_filename(struct argv_list *res, char *file,
                            struct d_string *buffer, char *pattern)
{
    bool match_dot =
        exp_opts.dotglob && strcmp(file, ".") != 0 && strcmp(file, "..") != 0;
    if ((file[0] != '.' || match_dot)
        && fnmatch(pattern, file, exp_opts.flags) == 0)
    {
        string_append(buffer, file);
        if (string_len(buffer) > 0)
            argv_list_append(res, string_value(buffer));
    }
}

void search_files(char *path, char *pattern, struct argv_list *res,
                  bool prepend)
{
    struct d_string *buffer = string_create(prepend ? path : "");
    size_t buf_size = string_len(buffer);
    if (prepend && buf_size > 1 && string_value(buffer)[buf_size - 1] != '/')
        string_append_char(buffer, '/');
    size_t init_len = string_len(buffer);

    DIR *d = opendir(path);
    if (d)
    {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL)
        {
            append_filename(res, dir->d_name, buffer, pattern);
            string_truncate_at(buffer, init_len);
        }
        closedir(d);
    }

    string_destroy(buffer);
}

static bool is_pattern(char *word)
{
    size_t i = 0;
    while (word[i])
    {
        if (word[i] == '?' || word[i] == '*' || word[i] == '[')
            return true;
        i++;
    }
    return false;
}

struct argv_list *return_expand_pathname(struct argv_list *res)
{
    if (res->len == 1 && strcmp(res->argv[0], "") == 0)
    {
        argv_list_free(res);
        return NULL;
    }

    return res;
}

static void set_options(struct sh_data *data)
{
    exp_opts.nocaseglob = data->options->nocaseglob;
    exp_opts.dotglob = data->options->dotglob;
    exp_opts.extglob = data->options->extglob;

    exp_opts.flags = 0;
    if (exp_opts.nocaseglob)
        exp_opts.flags |= FNM_CASEFOLD;
    if (exp_opts.extglob)
        exp_opts.flags |= FNM_EXTMATCH;
}

struct argv_list *expand_pathname(char *word, struct sh_data *data)
{
    set_options(data);
    if (!is_pattern(word))
        return NULL;

    char *word_copy = strdup(word);

    bool starts_slash = word_copy[0] == '/';
    char *tok = strtok(word_copy, "/");

    // Entry point is tok from here
    struct argv_list *res = argv_list_init();

    char *entry_point = starts_slash ? "/" : tok;
    tok = starts_slash ? tok : strtok(NULL, "/");

    if (tok == NULL)
        search_files(".", entry_point, res, false);
    else
        search_files(entry_point, tok, res, true);

    tok = strtok(NULL, "/");
    while (tok != NULL)
    {
        size_t len = res->len;
        for (size_t i = 0; i < len; i++)
        {
            search_files(res->argv[i], tok, res, true);
            *(res->argv[i]) = '\0';
        }
        tok = strtok(NULL, "/");
    }

    free(word_copy);

    sort_args(res);

    return return_expand_pathname(res);
}
