#include "parser3.h"

struct ast_node_pipe *init_pipe(void)
{
    struct ast_node_pipe *node;
    node = malloc(sizeof(struct ast_node_pipe));

    if (node == NULL)
        return NULL;

    node->node_base.type = PIPE_COMMAND;
    node->is_neg = 0;
    node->commands = NULL;
    return node;
}

struct ast_node_pipe *parse_pipe_loop(struct ast_node_pipe *node,
                                      struct lexer *lexer, size_t size,
                                      size_t capcity)
{
    struct ast_node_command *command = parse_command(lexer);

    while (command != NULL)
    {
        node->commands[size] = command;
        size++;

        if (size >= capcity)
        {
            capcity *= 2;
            node->commands =
                realloc(node->commands, sizeof(struct ast_node_if *) * capcity);
            if (node->commands == NULL)
            {
                free_ast_node_pipe(node);
                return NULL;
            }
        }

        struct token *token = get_token(lexer);
        if (token == NULL || token->type != PIPE)
            break;

        eat_token(lexer);
        command = parse_command(lexer);

        if (command == NULL)
        {
            fprintf(stderr, "syntax error: unexpected end of file\n");
            node->commands[size] = NULL;
            free_ast_node_pipe(node);
            errno = 2;
            return NULL;
        }
    }

    if (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
        eat_token(lexer);

    node->commands[size] = NULL;

    if (size == 0)
    {
        free(node->commands);
        free(node);
        return NULL;
    }
    return node;
}

struct ast_node_pipe *parse_pipe(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    size_t capcity = 16;
    size_t size = 0;

    struct ast_node_pipe *node = init_pipe();
    if (node == NULL)
        return NULL;

    if (get_token(lexer) != NULL && get_token(lexer)->type == NEG)
    {
        node->is_neg = 1;
        eat_token(lexer);
    }

    node->commands = malloc(sizeof(struct ast_node_if *) * capcity);
    if (node->commands == NULL)
    {
        free(node);
        return NULL;
    }

    node = parse_pipe_loop(node, lexer, size, capcity);
    if (node == NULL)
        return NULL;

    return node;
}

void free_ast_node_pipe(struct ast_node_pipe *node)
{
    if (node == NULL)
        return;

    if (node->commands == NULL)
    {
        free(node);
        return;
    }

    for (size_t i = 0; node->commands[i] != NULL; i++)
    {
        free_ast_node_command(node->commands[i]);
    }

    free(node->commands);
    free(node);
}

void print_ast_node_pipe(struct ast_node_pipe *node)
{
    if (node == NULL)
        return;

    if (node->commands == NULL)
        return;

    print_log("[ ");
    if (node->is_neg)
        print_log("! ");

    print_ast_node_command(node->commands[0]);
    for (size_t i = 1; node->commands[i] != NULL; i++)
    {
        print_log(" | ");
        print_ast_node_command(node->commands[i]);
    }

    print_log(" ]");
}
