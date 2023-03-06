#include "parser3.h"

struct ast_node_and_or *init_and_or(size_t capacity)
{
    struct ast_node_and_or *node;
    node = malloc(sizeof(struct ast_node_and_or));

    if (node == NULL)
        return NULL;

    node->node_base.type = AND_OR;
    node->pipes = malloc(sizeof(struct ast_node_pipe *) * capacity);
    node->ops = malloc(sizeof(enum op) * capacity);

    if (node->pipes == NULL || node->ops == NULL)
    {
        free(node->pipes);
        free(node->ops);
        free(node);
        return NULL;
    }

    return node;
}

void *handle_error(struct ast_node_and_or *node, size_t size)
{
    fprintf(stderr, "syntax error: unexpected end of file\n");
    node->pipes[size] = NULL;
    free_ast_node_and_or(node);
    errno = 2;
    return NULL;
}

struct ast_node_and_or *relloc_pipes(struct ast_node_and_or *node,
                                     size_t capacity, size_t size)
{
    if (size >= capacity)
    {
        capacity *= 2;
        node->pipes =
            realloc(node->pipes, sizeof(struct ast_node_pipe *) * capacity);
        if (node->pipes == NULL)
        {
            free_ast_node_and_or(node);
            return NULL;
        }
    }
    return node;
}

struct ast_node_and_or *realloc_op(struct ast_node_and_or *node, size_t op_cap,
                                   size_t op_size)
{
    if (op_size >= op_cap)
    {
        op_cap *= 2;
        node->ops = realloc(node->ops, sizeof(enum op) * op_cap);
        if (node->ops == NULL)
        {
            free_ast_node_and_or(node);
            return NULL;
        }
    }

    return node;
}

struct ast_node_and_or *parse_and_or(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    size_t capacity = 16;
    size_t size = 0;

    size_t op_cap = capacity;
    size_t op_size = 0;

    struct ast_node_and_or *node = init_and_or(capacity);
    if (node == NULL)
        return NULL;

    struct ast_node_pipe *pipe = parse_pipe(lexer);

    while (pipe != NULL)
    {
        node->pipes[size] = pipe;
        size++;

        struct token *token = get_token(lexer);
        if (token == NULL || (token->type != AND && token->type != OR))
            break;

        enum op operand = and_op;
        if (token->type == OR)
            operand = or_pop;

        node->ops[op_size] = operand;
        op_size++;

        node = relloc_pipes(node, capacity, size);
        node = realloc_op(node, op_cap, op_size);
        if (node == NULL)
            return NULL;

        eat_token(lexer);
        pipe = parse_pipe(lexer);

        if (pipe == NULL)
            return handle_error(node, size);
    }

    while (get_token(lexer) != NULL && get_token(lexer)->type == NEWLINE)
        eat_token(lexer);

    if (size == 0)
    {
        free(node->ops);
        free(node->pipes);
        free(node);
        return NULL;
    }

    node->nb_ops = op_size;
    node->pipes[size] = NULL;
    return node;
}

void free_ast_node_and_or(struct ast_node_and_or *node)
{
    if (node == NULL)
        return;

    if (node->pipes != NULL)
    {
        for (size_t i = 0; node->pipes[i] != NULL; i++)
        {
            free_ast_node_pipe(node->pipes[i]);
        }

        free(node->pipes);
    }

    if (node->ops != NULL)
    {
        free(node->ops);
    }

    free(node);
}

void print_ast_node_and_or(struct ast_node_and_or *node)
{
    if (node == NULL)
        return;
    // size_t i;
    // for (i = 0; node->pipes[i] != NULL; i++);
    // printf("nb of and_ors: %ld\n", i);
    print_ast_node_pipe(node->pipes[0]);

    for (size_t i = 0; i < node->nb_ops; i++)
    {
        if (node->ops[i] == and_op)
        {
            print_log(" AND ");
        }
        else
        {
            print_log(" OR ");
        }
        print_ast_node_pipe(node->pipes[i + 1]);
    }
}
