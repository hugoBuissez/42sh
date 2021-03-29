#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "argv_list.h"
#include "builtins.h"
#include "eval.h"
#include "exec.h"
#include "list.h"
#include "parser.h"
#include "shell.h"

int exec_simple_command(struct argv_list *arguments, struct sh_data *data)
{
    char **argv = arguments->argv;

    if (is_builtin(argv[0]))
    {
        enum eval_status rc = exec_builtin(arguments, data);
        return rc;
    }

    int pid = fork();
    if (pid == -1)
        errx(1, "fork failed");

    if (pid == 0) // Child
    {
        execvp(argv[0], argv);
        fclose(data->options->stream); // Prevent undefined behavior with stream

        warn("%s", argv[0]);
        exit(1);
    }

    int wstatus;
    waitpid(pid, &wstatus, 0);

    if (WEXITSTATUS(wstatus) != 0)
        return EVAL_NOT_FOUND;

    return EVAL_TRUE;
}
