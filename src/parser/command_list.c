#include "parser3.h"

struct ast_node_command_list *init_command_list(size_t capacity)
{
    struct ast_node_command_list *node;
    node = malloc(sizeof(struct ast_node_command_list));

    if (node == NULL)
        return NULL;

    node->node_base.type = COMMAND_LIST;

    node->and_ors = malloc(sizeof(struct ast_node_and_or *) * capacity);
    if (node->and_ors == NULL)
    {
        free(node);
        return NULL;
    }

    return node;
}

struct ast_node_command_list *parse_command_list(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    size_t capacity = 16;
    size_t size = 0;
    struct ast_node_command_list *node = init_command_list(capacity);
    if (node == NULL)
        return NULL;

    struct ast_node_and_or *and_or = parse_and_or(lexer);

    while (and_or != NULL)
    {
        node->and_ors[size] = and_or;
        size++;

        if (size >= capacity)
        {
            capacity *= 2;
            node->and_ors = realloc(
                node->and_ors, sizeof(struct ast_node_and_or *) * capacity);
            if (node->and_ors == NULL)
            {
                free_ast_node_command_list(node);
                return NULL;
            }
        }

        struct token *token = get_token(lexer);
        if (token == NULL || token->type != SEMICOLON)
            break;

        eat_token(lexer);
        and_or = parse_and_or(lexer);
    }

    while (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
        eat_token(lexer);

    if (size == 0)
    {
        free(node->and_ors);
        free(node);
        return NULL;
    }

    node->and_ors[size] = NULL;
    return node;
}

void free_ast_node_command_list(struct ast_node_command_list *node)
{
    if (node == NULL)
        return;

    struct ast_node_and_or **and_ors = node->and_ors;
    if (and_ors == NULL)
        return;

    for (size_t i = 0; and_ors[i] != NULL; i++)
    {
        free_ast_node_and_or(and_ors[i]);
    }

    free(and_ors);
    free(node);
}

void print_ast_node_command_list(struct ast_node_command_list *node)
{
    if (node == NULL)
        return;

    struct ast_node_and_or **and_ors = node->and_ors;

    if (and_ors == NULL)
        return;

    print_log("{ ");
    for (size_t i = 0; and_ors[i] != NULL; i++)
    {
        print_ast_node_and_or(and_ors[i]);
    }
    print_log(" }");
}
