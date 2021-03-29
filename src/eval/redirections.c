#include "redirections.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"
#include "list.h"
#include "parser.h"

struct redirect_operation
{
    int fd_dup;
    int initial_fd;
};

struct operator_flag_model
{
    const char *op;
    int flags;
};

static int get_fd(const char *operator)
{
    if (operator[0] == '>')
        return STDOUT_FILENO;

    return STDIN_FILENO;
}

static int get_correct_flags(const char *op)
{
    static struct operator_flag_model op_flags[] = {
        { .op = ">", .flags = O_WRONLY | O_CREAT | O_TRUNC },
        { .op = ">|", .flags = O_WRONLY | O_CREAT | O_TRUNC },
        { .op = ">>", .flags = O_WRONLY | O_CREAT | O_APPEND },
        { .op = ">&", .flags = O_WRONLY | O_CREAT },

        { .op = "<", .flags = O_RDONLY },
        { .op = "<<", .flags = O_RDONLY },
        { .op = "<&", .flags = O_RDONLY },
        { .op = "<<-", .flags = O_RDONLY },

        { .op = "<>", .flags = O_RDWR | O_CREAT },
    };

    for (size_t i = 0; i < sizeof(op_flags) / sizeof(op_flags[0]); ++i)
        if (strcmp(op, op_flags[i].op) == 0)
            return op_flags[i].flags;

    return -1;
}

static struct redirect_operation *
apply_redirection(struct redirection *redirection)
{
    struct redirect_operation *redirect_op = calloc(1, sizeof(*redirect_op));
    if (redirect_op == NULL)
        errx(1, "Memory exhausted");

    if (redirection->io_number)
    {
        redirect_op->initial_fd = atoi(redirection->io_number);
        if (fcntl(redirect_op->initial_fd, F_GETFD) == -1)
        {
            free(redirect_op);
            return NULL;
        }
    }
    else
        redirect_op->initial_fd = get_fd(redirection->operator);

    int fd;
    redirect_op->fd_dup = dup(redirect_op->initial_fd);

    fd = open(redirection->word, get_correct_flags(redirection->operator),
              0664 | O_CLOEXEC);

    if (fd == -1)
    {
        warn("%s", redirection->word);
        free(redirect_op);
        return NULL;
    }

    dup2(fd, redirect_op->initial_fd);

    return redirect_op;
}

struct list *apply_redirections(struct list *redirections)
{
    if (redirections == NULL)
        return NULL;

    struct list *redirect_operations = list_init();
    struct list_iterator *iterator = iterator_init(redirections);

    struct redirection *redirection = NULL;

    while ((redirection = iterator_next(iterator)))
    {
        struct redirect_operation *redir_op = apply_redirection(redirection);
        if (redir_op == NULL)
        {
            restore_redirections(redirect_operations);
            iterator_destroy(iterator);
            return NULL;
        }
        list_append(redirect_operations, redir_op);
    }

    iterator_destroy(iterator);

    return redirect_operations;
}

void restore_redirections(struct list *redirect_operations)
{
    if (redirect_operations == NULL)
        return;

    struct list_iterator *iterator = iterator_init(redirect_operations);

    struct redirect_operation *redir_op = NULL;
    while ((redir_op = iterator_next(iterator)))
    {
        dup2(redir_op->fd_dup, redir_op->initial_fd);
        close(redir_op->fd_dup);
    }

    iterator_destroy(iterator);

    list_free_elements(redirect_operations, free);
    list_destroy(redirect_operations);
}
