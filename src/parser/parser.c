#include "parser3.h"

struct ast_node *parse(struct lexer *lexer)
{
    return (struct ast_node *)parse_command_list(lexer);
}

void print_ast_node(struct ast_node *node)
{
    if (node == NULL)
        return;
    switch (node->type)
    {
    case SIMPLE_COMMAND:
        print_ast_node_simple_command((struct ast_node_simple_command *)node);
        break;
    case COMMAND_LIST:
        print_ast_node_command_list((struct ast_node_command_list *)node);
        break;
    case IF_COMMAND:
        print_ast_node_if((struct ast_node_if *)node);
        break;
    case PIPE_COMMAND:
        print_ast_node_pipe((struct ast_node_pipe *)node);
        break;
    default:
        break;
    }
}

void free_ast_node(struct ast_node *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case SIMPLE_COMMAND:
        free_ast_node_simple_command((struct ast_node_simple_command *)node);
        break;
    case COMMAND_LIST:
        free_ast_node_command_list((struct ast_node_command_list *)node);
        break;
    case IF_COMMAND:
        free_ast_node_if((struct ast_node_if *)node);
        break;
    case PIPE_COMMAND:
        free_ast_node_pipe((struct ast_node_pipe *)node);
        break;
    default:
        break;
    }
}
