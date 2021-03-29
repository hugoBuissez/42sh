#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "options.h"

int builtin_source(struct argv_list *args, struct sh_data *sh_data)
{
    FILE *stream = NULL;

    if (args->len == 1)
    {
        warnx("usage: source filename [arguments]");
        return 2;
    }

    if ((stream = fopen(args->argv[1], "r")) == NULL)
    {
        warn("%s", args->argv[1]);
        return 1;
    }

    struct sh_data new_sh_data;
    new_sh_data.options = parse_options(args->len, args->argv);
    new_sh_data.current_input = NULL;
    setup_sub_shell(&new_sh_data, sh_data);

    int rc = run_shell(&new_sh_data);

    end_sub_shell(&new_sh_data);

    return rc;
}
