#include "exec.h"

#include <unistd.h>

#include "../builtins/builtins.h"

struct builtin builtins[] = { { "echo", echo },
                              { "cd", cd },
                              { "exit", my_exit } };

int exec_fork(char **args)
{
    pid_t pid = fork();
    int status;

    // Error handling
    if (pid < 0)
        return -1;

    if (pid == 0) // Child Process
    {
        int exit_code = execvp(args[0], args);

        if (exit_code < 0)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(127);
        }

        exit(exit_code);
    }
    else
    {
        if (waitpid(pid, &status, 0) < 0)
        {
            return 2;
        }
        int exit_code = WEXITSTATUS(status);
        return exit_code;
    }
    return 0;
}

int exec_simple_command(struct ast_node_simple_command *node)
{
    if (node == NULL)
        return 2;

    char **args = node->words;
    if (args == NULL)
        return 2;

    if (strcmp(args[0], "true") == 0)
        return 0;

    if (strcmp(args[0], "false") == 0)
        return 1;

    static int builtins_nb = sizeof(builtins) / sizeof(struct builtin);
    for (int i = 0; i < builtins_nb; i++)
    {
        if (strcmp(args[0], builtins[i].name) == 0)
        {
            return builtins[i].func(args);
        }
    }

    return exec_fork(args);
}

int exec_if(struct ast_node_if *node)
{
    int exit_code =
        exec_command_list((struct ast_node_command_list *)node->condition);
    if (exit_code == 0)
        return exec_command_list((struct ast_node_command_list *)node->if_body);
    else
        return exec_command_list(
            (struct ast_node_command_list *)node->else_body);
}

int exec_while(struct ast_node_while *node)
{
    int exit_code = 0;
    while (exec_command_list((struct ast_node_command_list *)node->condition)
           == 0)
        exit_code =
            exec_command_list((struct ast_node_command_list *)node->loop);
    return exit_code;
}

int exec_until(struct ast_node_until *node)
{
    int exit_code = 0;
    while (exec_command_list((struct ast_node_command_list *)node->condition)
           != 0)
        exit_code =
            exec_command_list((struct ast_node_command_list *)node->loop);
    return exit_code;
}

int exec_for(struct ast_node_for *node)
{
    int exit_code = 0;
    if (node->list == NULL)
        return exit_code;
    for (size_t i = 0; node->list[i] != NULL; i++)
    {
        exit_code =
            exec_command_list((struct ast_node_command_list *)node->body);
    }

    return exit_code;
}

int exec_command(struct ast_node_command *node)
{
    if (node == NULL)
        return 0;

    switch (node->type)
    {
    case if_cmd:
        return exec_if(node->value.if_node);
        break;
    case simp_cmd:
        return exec_simple_command(node->value.simp_node);
        break;
    case while_cmd:
        return exec_while(node->value.while_node);
        break;
    case until_cmd:
        return exec_until(node->value.until_node);
        break;
    case for_cmd:
        return exec_for(node->value.for_node);
    default:
        break;
    }

    return 0;
}

int exec_pipeline(struct ast_node_pipe *node)
{
    if (node == NULL)
        return 2;

    int pipefd[2];
    int exit_code = 0;
    pid_t pid;
    int status;
    int n = 0;
    while (node->commands[n] != NULL)
        n += 1;

    if (n == 1)
    {
        if (node->is_neg)
            return !exec_command(node->commands[0]);
        return exec_command(node->commands[0]);
    }

    for (int i = 0; i < n - 1; i++)
    {
        // Create the pipe
        if (pipe(pipefd) < 0)
        {
            fprintf(stderr, "Failed to create pipe");
            exit(1);
        }

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Failed to fork process");
            exit(1);
        }

        if (pid == 0)
        {
            // Child process
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);

            exit_code = exec_command(node->commands[i]);
            exit(exit_code);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
        }
    }

    exit_code = exec_command(node->commands[n - 1]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        exit_code = WEXITSTATUS(status);
    }

    if (node->is_neg)
    {
        return !exit_code;
    }
    return exit_code;
}

int exec_and_or(struct ast_node_and_or *node)
{
    if (node == NULL)
        return 1;

    if (node->nb_ops == 0)
        return exec_pipeline(node->pipes[0]);

    int exit_code = exec_pipeline(node->pipes[0]);

    for (size_t i = 0; i < node->nb_ops; i++)
    {
        if ((node->ops[i] == and_op && exit_code == 0)
            || (node->ops[i] == or_pop && exit_code != 0))
            exit_code = exec_pipeline(node->pipes[i + 1]);
    }

    return exit_code;
}

int exec_command_list(struct ast_node_command_list *node)
{
    if (node == NULL)
        return 0;

    struct ast_node_and_or **and_ors = node->and_ors;
    int code = 0;

    for (int i = 0; and_ors[i] != NULL; i++)
        code = exec_and_or(and_ors[i]);

    return code;
}
