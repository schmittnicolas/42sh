#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void echo(char **args)
{
    size_t i = 0;
    int flags[3] = {0};

    while (args[i] != NULL && (strcmp(args[i],"-n") == 0) || (strcmp(args[i],"-e") == 0) || (strcmp(args[i],"-E") == 0))
    {
        if (strcmp(args[i],"-n") == 0)
            flags[0] = 1;
        else if (strcmp(args[i],"-e") == 0)
            flags[1] = 1;
        else
            flags[2] = 1;
        i++;
    }

    while (args[i] != NULL)
    {
        fputs(args[i],stdout);
        if (args[i+1] != NULL)
        {
            fputc(' ',stdout);
        }
        i++;
    }

    if (flags[0] == 0)
    {
        fputc('\n',stdout);
    }
}

int main(int argc, char *argv[])
{
    echo(argv);
    return 0;
}
