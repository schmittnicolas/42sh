#include <stdio.h>
#include <stdlib.h>

#include "exec/exec.h"
#include "parser/parser.h"
#include "io/stream.h"

int sh_loop(FILE* stream)
{
    char *line = get_user_input(stream);
    struct lexer *lexer = init_lexer(line);
    struct ast_node_simple_command *node = parse_simple_command(lexer);
    print_ast_node_simple_command(node);

    if (node != NULL)
    {
        exec_simple_command(node);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        int exit_code = 0;
        while (exit_code == 0)
        {
            printf("42sh$ ");
            exit_code = sh_loop(stdin);
        }

        return 0;
    }
    else if (argc == 3)
    {
        FILE* stream = NULL;

        if (strcmp(argv[1], "-c") == 0)
        {
            stream = create_stream_from_string(argv[2]);
        }
        else if (strcmp(argv[1], "-e") == 0)
        {
            stream = create_stream_from_file(argv[2]);
        }
        else
        {
            return 1;
        }

        int exit_code = sh_loop(stream);
        return exit_code;
    }
}
