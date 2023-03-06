#include "parser3.h"

struct ast_node_until *parse_until(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    struct ast_node_until *node;
    node = malloc(sizeof(struct ast_node_until));

    if (node == NULL)
        return NULL;

    node->node_base.type = UNTIL_COMMAND;
    node->condition = NULL;
    node->loop = NULL;

    /* Condition */

    struct token *token = get_token(lexer);

    if (token == NULL || token->type != UNTIL)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected 'until'");
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

    /* Until Loop */

    token = get_token(lexer);
    if (token == NULL || token->type != DO)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'do'");
        free_ast_node_until(node);
        return NULL;
    }

    eat_token(lexer);

    node->loop = (struct ast_node *)parse_compound_list(lexer);
    if (node->loop == NULL)
    {
        free_ast_node_until(node);
        return NULL;
    }

    /* Done */

    token = get_token(lexer);
    if (token == NULL || token->type != DONE)
    {
        errno = 2;
        fprintf(stderr, "syntax error: expected token 'done'");
        free_ast_node_until(node);
        return NULL;
    }

    eat_token(lexer);

    return node;
}

void free_ast_node_until(struct ast_node_until *node)
{
    if (node == NULL)
        return;

    free_ast_node(node->condition);
    free_ast_node(node->loop);
    free(node);
}

void print_ast_node_until(struct ast_node_until *node)
{
    if (node == NULL)
        return;

    print_log("UNTIL { ");
    print_ast_node(node->condition);
    print_log(" } DO { ");
    print_ast_node(node->loop);
    print_log(" } DONE");
}
