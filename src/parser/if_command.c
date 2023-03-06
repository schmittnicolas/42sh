#include "parser3.h"

struct ast_node_if *parse_condition(struct lexer *lexer,
                                    struct ast_node_if *node)
{
    struct token *token = get_token(lexer);
    if (token == NULL || (token->type != IF && token->type != ELIF))
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'if' or 'elif'");
        free(node);
        return NULL;
    }
    eat_token(lexer);

    node->condition = (struct ast_node *)parse_compound_list(lexer);
    if (node->condition == NULL)
    {
        free(node);
        return NULL;
    }

    return node;
}

struct ast_node_if *parse_if_body(struct lexer *lexer, struct ast_node_if *node)
{
    struct token *token = get_token(lexer);
    if (token == NULL || (token->type != THEN))
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'then'");
        errno = 2;
        free_ast_node_if(node);
        return NULL;
    }
    eat_token(lexer);

    node->if_body = (struct ast_node *)parse_compound_list(lexer);
    if (node->if_body == NULL)
    {
        free_ast_node_if(node);
        return NULL;
    }

    return node;
}

struct ast_node_if *parse_else_body(struct lexer *lexer,
                                    struct ast_node_if *node)
{
    struct token *token = get_token(lexer);
    if (token != NULL && token->type == ELIF)
    {
        node->else_body = (struct ast_node *)parse_if_command(lexer);
        if (node->else_body == NULL)
        {
            free_ast_node_if(node);
            return NULL;
        }

        return node;
    }

    else if (token != NULL && token->type == ELSE)
    {
        eat_token(lexer);
        node->else_body = (struct ast_node *)parse_compound_list(lexer);
        if (node->else_body == NULL)
        {
            free_ast_node_if(node);
            return NULL;
        }

        /* Fi */

        token = get_token(lexer);
        if (token == NULL || token->type != FI)
        {
            errno = 2;
            fprintf(stderr, "syntax error: expected token 'fi'");
            free_ast_node_if(node);
            return NULL;
        }

        eat_token(lexer);

        return node;
    }

    /* Fi */

    token = get_token(lexer);
    if (token == NULL || token->type != FI)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'fi'");
        free_ast_node_if(node);
        return NULL;
    }

    eat_token(lexer);
    return node;
}

struct ast_node_if *parse_if_command(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    struct ast_node_if *node;
    node = malloc(sizeof(struct ast_node_if));

    if (node == NULL)
        return NULL;

    node->node_base.type = IF_COMMAND;
    node->condition = NULL;
    node->else_body = NULL;
    node->if_body = NULL;

    /* Condition */

    node = parse_condition(lexer, node);
    if (node == NULL)
        return NULL;

    /* If body */

    node = parse_if_body(lexer, node);
    if (node == NULL)
        return NULL;

    /* Else body (optional) */

    node = parse_else_body(lexer, node);
    if (node == NULL)
        return NULL;

    return node;
}

void print_ast_node_if(struct ast_node_if *node)
{
    if (node == NULL)
    {
        print_log("null");
        return;
    }

    print_log("IF { ");
    print_ast_node(node->condition);
    print_log(" } THEN { ");
    print_ast_node(node->if_body);
    print_log(" } ELSE { ");
    print_ast_node(node->else_body);
    print_log(" } FI");
}

void free_ast_node_if(struct ast_node_if *node)
{
    if (node == NULL)
        return;

    free_ast_node(node->condition);
    free_ast_node(node->if_body);
    free_ast_node(node->else_body);
    free(node);
}
