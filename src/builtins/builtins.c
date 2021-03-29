#include "builtins.h"

#include <err.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

typedef int (*builtin_func)(struct argv_list *, struct sh_data *);

struct builtin_model
{
    const char *name;
    builtin_func fnc;
};

static int return_break(struct argv_list *args, struct sh_data *data)
{
    args = args;
    data = data;
    return EVAL_BREAK;
}

static int return_continue(struct argv_list *args, struct sh_data *data)
{
    args = args;
    data = data;
    return EVAL_CONTINUE;
}

static struct builtin_model builtins[] = {
    { .name = "echo", .fnc = builtin_echo },
    { .name = "cd", .fnc = builtin_cd },
    { .name = "exit", .fnc = builtin_exit },
    { .name = "export", .fnc = builtin_export },
    { .name = "break", .fnc = return_break },
    { .name = "continue", .fnc = return_continue },
    { .name = "source", .fnc = builtin_source },
    { .name = ".", .fnc = builtin_source },
    { .name = "alias", .fnc = builtin_alias },
    { .name = "unalias", .fnc = builtin_unalias },
};

bool is_builtin(char *cmd)
{
    if (!cmd)
        return false;

    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); ++i)
        if (strcmp(cmd, builtins[i].name) == 0)
            return true;

    return false;
}

int exec_builtin(struct argv_list *args, struct sh_data *data)
{
    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++)
        if (strcmp(args->argv[0], builtins[i].name) == 0)
            return builtins[i].fnc(args, data);

    warnx("%s not found for execution.", args->argv[0]);
    return 1;
}
