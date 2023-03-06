#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

int cd(char **args)
{
    char *old_pwd;
    char *dest;

    if (args[2])
    {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    }

    if (args[1] == NULL)
    {
        dest = getenv("HOME");
    }
    else if ((strcmp(args[1], "-")) == 0)
    {
        old_pwd = getenv("OLDPWD");
        if (old_pwd == NULL)
        {
            fprintf(stderr, "OLDPWD not set\n");
            return 1;
        }
        dest = old_pwd;
        fprintf(stdout, "%s\n", old_pwd);
    }
    else
    {
        dest = args[1];
    }

    old_pwd = getcwd(NULL, 0);

    if (chdir(dest) != 0)
    {
        free(old_pwd);
        fprintf(stderr, "No such file or directory");
        return 1;
    }

    setenv("OLDPWD", old_pwd, 1);
    setenv("PWD", dest, 1);

    free(old_pwd);
    return 0;
}
