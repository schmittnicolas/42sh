#ifndef BUILTIN_H
#define BUILTIN_H

struct builtin
{
    char *name;
    void (*func)(char **args);
};

#endif /* BUILTIN_H*/
