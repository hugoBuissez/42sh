#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "eval.h"
#include "exec.h"

struct p_info
{
    int *fds;
    int close_fd;
    int dup_fd;
    int redirect_to;
};

static int exec_child(struct p_info *p_info, struct ast_node *node,
                      struct argv_list *args, struct sh_data *data)
{
    int pid = fork();
    if (pid == -1)
        errx(1, "fork failed");

    if (pid == 0)
    {
        close(p_info->fds[p_info->close_fd]);
        if (dup2(p_info->fds[p_info->dup_fd], p_info->redirect_to) == -1)
            errx(1, "dup2 failed");

        int left_rc = eval_node(node, args, data);
        exit(left_rc);
    }

    return pid;
}

int exec_pipe(struct ast_node *left, struct ast_node *right,
              struct argv_list *args, struct sh_data *data)
{
    int wstatus;
    int fds[2];
    if (pipe(fds) == -1)
        errx(1, "pipe failed");

    struct p_info l_p_info = {
        .fds = fds, .close_fd = 0, .dup_fd = 1, .redirect_to = STDOUT_FILENO
    };
    int left_pid = exec_child(&l_p_info, left, args, data);

    struct p_info r_p_info = {
        .fds = fds, .close_fd = 1, .dup_fd = 0, .redirect_to = STDIN_FILENO
    };
    int right_pid = exec_child(&r_p_info, right, args, data);

    close(fds[0]);
    close(fds[1]);
    waitpid(left_pid, &wstatus, 0);
    waitpid(right_pid, &wstatus, 0);

    return WEXITSTATUS(wstatus);
}
