#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "expansion.h"
#include "options.h"
#include "string_utils.h"

static void exec_subshell(int *fds, char *input, struct sh_data *data)
{
    int wstatus;

    int pid = fork();
    if (pid == -1)
        errx(1, "fork failed");

    if (pid == 0)
    {
        close(fds[0]);
        if (dup2(fds[1], STDOUT_FILENO) == -1)
            errx(1, "dup2 failed");

        struct sh_data new_sh_data;
        new_sh_data.options = parse_options(0, NULL);
        new_sh_data.options->stream = fmemopen(input, strlen(input), "r");
        setup_sub_shell(&new_sh_data, data);

        int exit_code = run_shell(&new_sh_data);

        end_sub_shell(&new_sh_data);

        exit(exit_code);
    }

    waitpid(pid, &wstatus, 0);
    data->rc = WEXITSTATUS(wstatus);

    close(fds[1]);
}

char *eval_command_sub(char *input, struct sh_data *data)
{
    int fds[2];
    if (pipe(fds) == -1)
        errx(1, "pipe failed");

    exec_subshell(fds, input, data);

    char buf[4096];
    char *cmd_ouput = NULL;
    size_t r = 0;
    while ((r = read(fds[0], buf, sizeof(buf) - 1)) != 0)
    {
        buf[r] = '\0';
        str_append(&cmd_ouput, buf);
    }
    close(fds[0]);

    return cmd_ouput;
}
