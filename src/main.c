#include <stdio.h>
#include <stdlib.h>

#include "exec/exec.h"
#include "io/stream.h"
#include "parser/parser3.h"

int sh_loop(FILE *stream)
{
    struct lexer *lexer = init_lexer(stream);
    if (lexer == NULL)
        return 1;

    int exit_code = 0;
    while (!is_empty(lexer))
    {
        struct ast_node *node = parse(lexer);

        if (errno == 2)
        {
            while (!is_empty(lexer))
                eat_token(lexer);
            free_lexer(lexer);
            free_ast_node(node);
            return 2;
        }

        print_ast_node(node);
        print_log("\n");
        exit_code = exec_command_list((struct ast_node_command_list *)node);
        fflush(stdout);
        free_ast_node(node);
    }

    free_lexer(lexer);
    return exit_code;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        /* Read from stdin */

        return sh_loop(stdin);
    }
    else if (argc == 2)
    {
        /* Read from a file */

        FILE *stream = fopen(argv[1], "r");

        if (!stream)
        {
            fprintf(stderr, "Failed to open file %s\n", argv[2]);
            return 1;
        }

        int code = sh_loop(stream);
        fclose(stream);
        return code;
    }
    else if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        /* Read from a string */

        FILE *stream = fmemopen(argv[2], strlen(argv[2]), "r");

        if (!stream)
        {
            fprintf(stderr, "fmemopen: faild to convert string to file\n");
            return 1;
        }

        int code = sh_loop(stream);
        fclose(stream);
        return code;
    }

    return 2;
}
