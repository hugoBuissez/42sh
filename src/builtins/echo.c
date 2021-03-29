#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "argv_list.h"
#include "builtins.h"

/**
 * @Synopsis  Print the special matching character
 *
 * @Param letter The letter to match
 */
static void print_escapes(char letter)
{
    if (letter == 'n')
        putchar('\n');
    else if (letter == '\\')
        putchar('\\');
    else if (letter == 'a')
        putchar('\a');
    else if (letter == 'b')
        putchar('\b');
    else if (letter == 'e')
        putchar('\033');
    else if (letter == 'f')
        putchar('\f');
    else if (letter == 'r')
        putchar('\r');
    else if (letter == 't')
        putchar('\t');
    else if (letter == 'v')
        putchar('\v');
    else
        putchar(letter); // Print the letter (not a special character).
}

/**
 * @Synopsis Handle special character if present
 *
 * @Param argv The argument to check
 */
static void handle_escapes(const char *argv)
{
    int i = 0;
    while (argv[i])
    {
        if (argv[i] == '\\' && argv[i + 1])
        {
            print_escapes(argv[i + 1]);
            i++;
        }
        else
            printf("%c", argv[i]);
        i++;
    }
}

struct echo_options
{
    bool opt_n;
    bool opt_e;
};

struct echo_options get_echo_options(int argc, char *argv[], int *index)
{
    struct echo_options options;
    options.opt_n = false;
    options.opt_e = false;

    while (*index < argc && argv[*index][0] == '-')
    {
        if (strcmp(argv[*index], "-n") == 0)
            options.opt_n = true;
        else if (strcmp(argv[*index], "-e") == 0)
            options.opt_e = true;
        else if (strcmp(argv[*index], "-E") == 0)
            options.opt_e = false;
        else
            break;
        (*index)++;
    }

    return options;
}

/**
 * @Synopsis  Execute the echo builtin
 *
 * @Param argv The echo argument(s)
 *
 * @Returns The return code
 */
int builtin_echo(struct argv_list *args, struct sh_data *data)
{
    data = data;
    if (args->len == 1)
    {
        printf("\n");
        return 0;
    }

    int index = 1;
    struct echo_options options =
        get_echo_options(args->len, args->argv, &index);

    for (; args->argv[index] != NULL; ++index)
    {
        if (options.opt_e)
            handle_escapes(args->argv[index]);
        else
            printf("%s%s", args->argv[index],
                   args->argv[index + 1] != NULL ? " " : "");
    }

    if (!options.opt_n)
        printf("\n");

    fflush(stdout);

    return 0;
}
