#ifndef BUILTINS_H
#define BUILTINS_H

struct builtin
{
    char *name;
    int (*func)(char **args);
};

int echo(char **args);
int cd(char **args);
int my_exit(char **args);

#endif /* BUILTINS_H */
