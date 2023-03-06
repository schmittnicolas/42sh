#include <stdlib.h>

#include "builtins.h"

int my_exit(char **args)
{
    int exit_code = atoi(args[1]);
    exit(exit_code);
}
