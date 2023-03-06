#include "parser3.h"

struct ast_node_command *parse_command(struct lexer *lexer)
{
    struct token *token = get_token(lexer);

    if (token == NULL)
        return NULL;

    struct ast_node_command *node = malloc(sizeof(struct ast_node_command));
    if (node == NULL)
        return NULL;

    if (token->type == WORD)
    {
        node->type = simp_cmd;
        node->value.simp_node = parse_simple_command(lexer);
        if (node->value.simp_node == NULL)
        {
            free(node);
            node = NULL;
        }
    }
    else if (token->type == IF)
    {
        node->type = if_cmd;
        node->value.if_node = parse_if_command(lexer);
        if (node->value.if_node == NULL)
        {
            free(node);
            node = NULL;
        }
    }
    else if (token->type == WHILE)
    {
        node->type = while_cmd;
        node->value.while_node = parse_while(lexer);
        if (node->value.while_node == NULL)
        {
            free(node);
            node = NULL;
        }
    }
    else if (token->type == UNTIL)
    {
        node->type = until_cmd;
        node->value.until_node = parse_until(lexer);
        if (node->value.while_node == NULL)
        {
            free(node);
            node = NULL;
        }
    }
    else if (token->type == FOR)
    {
        node->type = for_cmd;
        node->value.for_node = parse_for(lexer);
        if (node->value.for_node == NULL)
        {
            free(node);
            node = NULL;
        }
    }
    else
    {
        free(node);
        node = NULL;
    }

    return node;
}

void free_ast_node_command(struct ast_node_command *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case simp_cmd:
        free_ast_node_simple_command(node->value.simp_node);
        break;
    case if_cmd:
        free_ast_node_if(node->value.if_node);
        break;
    case while_cmd:
        free_ast_node_while(node->value.while_node);
        break;
    case until_cmd:
        free_ast_node_until(node->value.until_node);
        break;
    case for_cmd:
        free_ast_node_for(node->value.for_node);
        break;
    default:
        break;
    }

    free(node);
}

void print_ast_node_command(struct ast_node_command *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case simp_cmd:
        print_ast_node_simple_command(node->value.simp_node);
        break;
    case if_cmd:
        print_ast_node_if(node->value.if_node);
        break;
    case while_cmd:
        print_ast_node_while(node->value.while_node);
        break;
    case until_cmd:
        print_ast_node_until(node->value.until_node);
        break;
    case for_cmd:
        print_ast_node_for(node->value.for_node);
        break;
    default:
        break;
    }
}
