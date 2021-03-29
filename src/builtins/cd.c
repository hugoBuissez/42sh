#include <err.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "variables.h"

static int warnx_ret(int ret_val, char *msg)
{
    warnx("%s", msg);
    return ret_val;
}

int builtin_cd(struct argv_list *args, struct sh_data *data)
{
    char old_path[PATH_MAX];
    char *trash = getcwd(old_path, PATH_MAX);
    char *path = NULL;

    if (args->len == 1)
        path = find_sh_variable("HOME", data);
    else if (args->len == 2)
    {
        if (strcmp(args->argv[1], "-") == 0)
        {
            path = find_sh_variable("OLDPWD", data);
            if (path == NULL)
                return warnx_ret(1, "cd: OLDPWD not set");
            puts(path);
            fflush(stdout);
        }
        else
            path = args->argv[1];
    }
    else if (args->len > 2)
        return warnx_ret(1, "cd: too many arguments");

    if (chdir(path) == -1)
    {
        warn("cd");
        return 1;
    }

    char curr_path[PATH_MAX];
    trash = getcwd(curr_path, PATH_MAX);
    trash = trash;
    set_sh_variable("PWD", curr_path, 1, data);
    set_sh_variable("OLDPWD", old_path, 1, data);

    return 0;
}
