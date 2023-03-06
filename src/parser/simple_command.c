#include "parser3.h"

struct ast_node_simple_command *parse_simple_command(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    struct ast_node_simple_command *node;
    node = malloc(sizeof(struct ast_node_command));

    if (node == NULL)
        return NULL;

    node->node_base.type = SIMPLE_COMMAND;

    size_t capacity = 16;
    size_t size = 0;

    node->words = malloc(sizeof(char *) * capacity);
    if (node->words == NULL)
    {
        free(node);
        return NULL;
    }

    struct token *token = get_token(lexer);

    if (token == NULL || token->type != WORD)
    {
        free(node->words);
        free(node);
        return NULL;
    }

    while (token != NULL && is_word_token(token->type))
    {
        node->words[size] = strdup(token->value);
        size++;

        if (size >= capacity)
        {
            capacity *= 2;
            node->words = realloc(node->words, sizeof(char *) * capacity);
            if (node->words == NULL)
            {
                free_ast_node_simple_command(node);
                return NULL;
            }
        }

        eat_token(lexer);
        token = get_token(lexer);
    }

    node->words[size] = NULL;

    return node;
}

void free_ast_node_simple_command(struct ast_node_simple_command *node)
{
    if (node == NULL)
        return;

    char **words = node->words;

    for (size_t i = 0; words[i] != NULL; i++)
    {
        free(words[i]);
    }

    free(words);
    free(node);
}

void print_ast_node_simple_command(struct ast_node_simple_command *node)
{
    if (node == NULL)
    {
        print_log("null");
        return;
    }

    assert(node->node_base.type == SIMPLE_COMMAND);
    char **words = node->words;

    if (words == NULL)
        return;

    print_log("%s", words[0]);
    for (size_t i = 1; words[i] != NULL; i++)
    {
        print_log(" \"%s\"", words[i]);
    }

    print_log(" ;");
}
