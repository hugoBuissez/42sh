#include "options.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"

static void parse_o_option(struct options *opts, char *option, int set)
{
    if (strcmp(option, "dotglob") == 0)
        opts->dotglob = set;
    else if (strcmp(option, "expand_aliases") == 0)
        opts->expand_aliases = set;
    else if (strcmp(option, "extglob") == 0)
        opts->extglob = set;
    else if (strcmp(option, "nocaseglob") == 0)
        opts->nocaseglob = set;
    else if (strcmp(option, "nullglob") == 0)
        opts->nullglob = set;
    else if (strcmp(option, "sourcepath") == 0)
        opts->sourcepath = set;
    else if (strcmp(option, "xpg_echo") == 0)
        opts->xpg_echo = set;
    else
        errx(2, "%s: invalid option name", option);
}

static int handle_o(int current_index, int argc, char *argv[],
                    struct options *opts)
{
    // Currently, we are just skipping +O and -O
    while (current_index < argc
           && (strcmp(argv[current_index], "-O") == 0
               || strcmp(argv[current_index], "+O") == 0))
    {
        current_index++;
        parse_o_option(opts, argv[current_index],
                       argv[current_index - 1][0] == '-');
        //-O or +O value will be argv[current_index]
        current_index++;
    }
    //-O and +O were skipped
    return current_index;
}

static int skip_empty_c(int current_index, char *argv[])
{
    // Skip all the -c
    while (argv[current_index + 1]
           && strcmp(argv[current_index + 1], "-c") == 0)
        current_index++;
    return current_index;
}

struct options *parse_options(int argc, char *argv[])
{
    struct options *options = calloc(1, sizeof(*options));
    if (options == NULL)
        errx(1, "Memory exhausted");

    options->args = argv_list_init();

    if (argv == 0 || argv == NULL)
        return options;

    int current_index = 1;
    current_index = handle_o(current_index, argc, argv, options);

    if (current_index == argc)
        options->stream = stdin;
    else if (strcmp(argv[current_index], "-c") == 0)
    // read from the string
    {
        current_index = skip_empty_c(current_index, argv);
        if (argv[current_index + 1] == NULL)
            errx(2, "Usage: ./42sh -c <command>");
        size_t input_len = strlen(argv[current_index + 1]);
        if (input_len > 0)
            options->stream = fmemopen(argv[current_index + 1], input_len, "r");
    }
    else
    // we have a filepath to open
    {
        options->input_is_file = true;
        const char *path = argv[current_index];
        if ((options->stream = fopen(path, "r")) == NULL)
            err(1, "%s", path);
    }

    return options;
}

void free_options(struct options *options)
{
    if (options->stream)
        fclose(options->stream);

    argv_list_free(options->args);
    free(options);
}
