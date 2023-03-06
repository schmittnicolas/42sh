#include "parser3.h"

char **parse_list(struct lexer *lexer)
{
    size_t size = 0;
    size_t capacity = 16;

    char **words = malloc(sizeof(char *) * capacity);
    if (words == NULL)
        return NULL;

    struct token *token = get_token(lexer);

    while (token != NULL && is_word_token(token->type))
    {
        words[size] = strdup(token->value);
        size++;

        if (size >= capacity)
        {
            capacity *= 2;
            words = realloc(words, sizeof(char *) * capacity);
            if (words == NULL)
                return NULL;
        }

        eat_token(lexer);
        token = get_token(lexer);
    }

    if (token != NULL && token->type == SEMICOLON)
        eat_token(lexer);

    words[size] = NULL;
    return words;
}

struct ast_node_for *parse_do(struct lexer *lexer, struct ast_node_for *node)
{
    struct token *token = get_token(lexer);

    if (token == NULL || token->type != DO)
    {
        free_ast_node_for(node);
        fprintf(stderr, "syntax error: expected token 'do'\n");
        errno = 2;
        return NULL;
    }

    eat_token(lexer);

    node->body = (struct ast_node *)parse_compound_list(lexer);
    if (node->body == NULL)
    {
        errno = 2;
        free_ast_node_for(node);
        fprintf(stderr, "syntax error: unexpected token");
        return NULL;
    }
    else
    {
        if (get_token(lexer) != NULL && get_token(lexer)->type == SEMICOLON)
            eat_token(lexer);
    }

    token = get_token(lexer);

    if (token == NULL || token->type != DONE)
    {
        free_ast_node_for(node);
        fprintf(stderr, "syntax error: expected token: 'done'\n");
        errno = 2;
        return NULL;
    }

    eat_token(lexer);

    return node;
}

struct ast_node_for *parse_for(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    struct ast_node_for *node = malloc(sizeof(struct ast_node_for));
    if (node == NULL)
        return NULL;

    node->node_base.type = FOR_COMMAND;
    node->list = NULL;
    node->variable = NULL;
    node->body = NULL;

    /* For */

    struct token *token = get_token(lexer);

    if (token == NULL || (token->type != FOR))
    {
        free_ast_node_for(node);
        fprintf(stderr, "syntax error: expected token 'for'\n");
        errno = 2;
        return NULL;
    }

    eat_token(lexer);

    /* Variable */

    token = get_token(lexer);

    if (token == NULL)
    {
        free_ast_node_for(node);
        fprintf(stderr, "syntax error: exepected identifier\n");
        errno = 2;
        return NULL;
    }

    node->variable = strdup(token->value);

    eat_token(lexer);

    /* In */

    token = get_token(lexer);

    if (token != NULL && token->type == IN)
    {
        eat_token(lexer);
        node->list = parse_list(lexer);
        if (node->list == NULL)
        {
            free_ast_node_for(node);
            return NULL;
        }
    }

    /* Do */

    node = parse_do(lexer, node);
    if (node == NULL)
        return NULL;

    return node;
}

void print_ast_node_for(struct ast_node_for *node)
{
    if (node == NULL)
        return;

    print_log("FOR %s IN ", node->variable);
    if (node->list != NULL)
    {
        for (size_t i = 0; node->list[i] != NULL; i++)
            print_log("%s ", node->list[i]);
    }
    print_ast_node(node->body);
}

void free_ast_node_for(struct ast_node_for *node)
{
    if (node == NULL)
        return;

    free(node->variable);
    free_ast_node(node->body);
    if (node->list != NULL)
    {
        for (size_t i = 0; node->list[i] != NULL; i++)
            free(node->list[i]);
    }

    free(node->list);
    free(node);
}
