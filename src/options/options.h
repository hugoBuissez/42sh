#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <stdbool.h>
#include <stdio.h>

struct options
{
    FILE *stream;
    bool input_is_file;
    /* ----- */
    char *print_output_file;
    struct argv_list *args;
    /* ----- */
    int dotglob;
    int expand_aliases;
    int extglob;
    int nocaseglob;
    int nullglob;
    int sourcepath;
    int xpg_echo;
};

struct options *parse_options(int argc, char *argv[]);
void free_options(struct options *options);

#endif
