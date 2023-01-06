#include "exec.h"

int exec_simple_command(struct ast_node_simple_command *node)
{
    if (node == NULL)
        return 1;

    char **args = node->words;
    pid_t pid = fork();

    /* Error handling */
    if (pid < 0)
        return -1;

    if (pid == 0) /* Child Process */
    {
        if (execvp(args[0], args) < 0)
            return 1;
        return 0;
    }
    else /* Parent Process */
    {
        int status;
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        return 0;
    }
}
