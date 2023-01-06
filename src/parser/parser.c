#include "parser.h"

struct ast_node_simple_command *parse_simple_command(struct lexer *lexer)
{
    /* Check if the lexer is not null */
    if (lexer == NULL)
        return NULL;

    /* Get the token and check if it is a valid token */
    struct token *token = get_next_token(lexer);

    if (token == NULL)
        return NULL;
    if (token->type != WORD)
        return NULL;
    
    /* Create an ast node for simple command */
    struct ast_node_simple_command *node = malloc(sizeof(struct ast_node_simple_command));
    if (node == NULL)
        return NULL;

    /* Create array to handle the words */
    size_t capacity = 2;
    size_t size = 0;

    char **words = malloc(sizeof(char *) * capacity);
    if (words == NULL)
        return NULL;


    while (token != NULL && token->type == WORD)
    {
        words[size] = token->value;
        size++;
        if (size >= capacity)
        {
            capacity *= 2;
            words = realloc(words, sizeof(char *) * capacity);
        }

        token = get_next_token(lexer);
    }

    words[size] = NULL;
    node->words = words;
    return node;
}

void free_ast_node_simple_command(struct ast_node_simple_command *node)
{
    if (node == NULL)
        return;

    char **words = node->words;

    for(size_t i = 0; words[i] != NULL; i++)
    {
        free(words[i]);
    }

    free(words);
    free(node);
}

void print_ast_node_simple_command(struct ast_node_simple_command *node)
{
    printf("AST: ");
    if (node == NULL)
        return;

    char **words = node->words;

    if (words == NULL)
        return;
    
    printf("%s { ", words[0]);

    for (size_t i = 1; words[i] != NULL; i++)
    {
        printf(" \"%s\"", words[i]);
    }

    printf(" }\n");
}

/*
int main(void)
{
    char *input = "echo this as an input";
    struct lexer *lexer = init_lexer(input);
    if (lexer == NULL)
        printf("NULL lexer\n");
    
    struct ast_node_simple_command *node = parse_simple_command(lexer);
    printf("ast created\n");
    print_ast_node_simple_command(node);
    return 0;
}
*/