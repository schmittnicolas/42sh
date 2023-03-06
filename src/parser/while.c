#include "parser3.h"

struct ast_node_while *parse_while(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    struct ast_node_while *node;
    node = malloc(sizeof(struct ast_node_while));

    if (node == NULL)
        return NULL;

    node->node_base.type = WHILE_COMMAND;
    node->condition = NULL;
    node->loop = NULL;

    /* Condition */

    struct token *token = get_token(lexer);

    if (token == NULL || token->type != WHILE)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected 'while'");
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

    /* While Loop */

    token = get_token(lexer);
    if (token == NULL || token->type != DO)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'do'");
        free_ast_node_while(node);
        return NULL;
    }

    eat_token(lexer);

    node->loop = (struct ast_node *)parse_compound_list(lexer);
    if (node->loop == NULL)
    {
        free_ast_node_while(node);
        return NULL;
    }

    /* Done */

    token = get_token(lexer);
    if (token == NULL || token->type != DONE)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'done'");
        free_ast_node_while(node);
        return NULL;
    }

    eat_token(lexer);

    return node;
}

void free_ast_node_while(struct ast_node_while *node)
{
    if (node == NULL)
        return;

    free_ast_node(node->condition);
    free_ast_node(node->loop);
    free(node);
}

void print_ast_node_while(struct ast_node_while *node)
{
    if (node == NULL)
        return;

    print_log("WHILE { ");
    print_ast_node(node->condition);
    print_log(" } DO { ");
    print_ast_node(node->loop);
    print_log(" } DONE");
}
