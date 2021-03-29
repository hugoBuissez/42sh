#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argv_list.h"
#include "builtins.h"

static bool check_arg_digit(const char *arg)
{
    for (size_t i = 0; arg[i]; i++)
    {
        if (isdigit(arg[i]) == 0)
            return false;
    }
    return true;
}

int builtin_exit(struct argv_list *args, struct sh_data *data)
{
    int exit_code = 0;

    if (args->argv[1] != NULL)
    {
        // Check if argument is numeric.
        if (!check_arg_digit(args->argv[1]))
            errx(2, "%s: foo: numeric argument required", args->argv[1]);

        if (args->len > 2)
        {
            warnx("exit: too many arguments");
            return 1;
        }

        fprintf(stderr, "exit\n");
        exit_code = atoi(args->argv[1]);
    }
    argv_list_free(args);
    end_shell(data); // Free shell resources

    exit(exit_code);

    return 1; // Should never reach this point
}
