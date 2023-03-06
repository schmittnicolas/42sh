#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Prints its arguments separated by spaces, and prints a final newline
 * Flags :
 * -e > enable the interpretation of "\n", "\t" and "\\" escapes
 * -E > disable the interpretation of "\n", "\t" and "\\" escapes
 * -n > n inhibits printing a newline.
 */
int echo(char **args)
{
    size_t i = 1;
    int flags[3] = { 0 };

    /* flag checking loop */
    while (args[i] != NULL
           && ((strcmp(args[i], "-n") == 0) || (strcmp(args[i], "-e") == 0)
               || (strcmp(args[i], "-E") == 0)))
    {
        if (strcmp(args[i], "-n") == 0)
            flags[0] = 1;
        else if (strcmp(args[i], "-e") == 0)
        {
            flags[1] = 1;
            flags[2] = 0;
        }
        else if (strcmp(args[i], "-E") == 0)
        {
            flags[2] = 1;
            flags[1] = 0;
        }
        i++;
    }

    while (args[i] != NULL)
    {
        /* if -e flag is used */
        if (flags[1] == 1)
        {
            while (*args[i] != '\0')
            {
                if (*args[i] == '\\')
                {
                    args[i]++;
                    if (*args[i])
                    {
                        if (*args[i] == 'n')
                            putchar('\n');
                        else if (*args[i] == 't')
                            putchar('\t');
                        else if (*args[i] == '\\')
                            putchar('\\');
                    }
                }
                else
                    putchar(*args[i]);

                args[i]++;
            }
        }
        /* same as if -E was used*/
        else
        {
            fputs(args[i], stdout);
        }

        if (args[i + 1] != NULL)
        {
            fputc(' ', stdout);
        }
        i++;
    }

    /* if -n flag is unused */
    if (flags[0] == 0)
    {
        fputc('\n', stdout);
    }

    return 0;
}
