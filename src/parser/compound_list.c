#include "parser3.h"

struct ast_node_command_list *init_compound_list(size_t capacity)
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

struct ast_node_command_list *parse_compound_list(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    size_t capcity = 16;
    size_t size = 0;

    struct ast_node_command_list *node = init_compound_list(capcity);

    struct token *token = get_token(lexer);

    if (token != NULL && token->type == NEWLINE)
        eat_token(lexer);

    struct ast_node_and_or *and_or = parse_and_or(lexer);

    if (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
        eat_token(lexer);

    while (and_or != NULL)
    {
        node->and_ors[size] = and_or;
        size++;

        if (size >= capcity)
        {
            capcity *= 2;
            node->and_ors = realloc(node->and_ors,
                                    sizeof(struct ast_node_and_or *) * capcity);
            if (node->and_ors == NULL)
            {
                free_ast_node_command_list(node);
                return NULL;
            }
        }

        struct token *token = get_token(lexer);
        if (token == NULL
            || (token->type != SEMICOLON && token->type != NEWLINE))
            break;

        if (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
            eat_token(lexer);

        eat_token(lexer);
        and_or = parse_and_or(lexer);
    }

    if (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
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
