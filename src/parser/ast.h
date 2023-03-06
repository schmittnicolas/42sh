#ifndef AST_H
#define AST_H

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../io/stream.h"
#include "../lexer/lexer.h"

enum node_type
{
    SIMPLE_COMMAND,
    COMMAND_LIST,
    IF_COMMAND,
    PIPE_COMMAND,
    COMMAND,
    WHILE_COMMAND,
    UNTIL_COMMAND,
    AND_OR,
    FOR_COMMAND,
};

enum op
{
    and_op,
    or_pop,
};

enum redir_type
{
    REDIR_INPUT,
    REDIR_OUTPUT,
};

struct ast_node
{
    enum node_type type;
};

struct ast_node_simple_command
{
    struct ast_node node_base;

    /* simple command field */
    int IO;

    char **words; // NULL terminated char* list
};

struct ast_node_for
{
    struct ast_node node_base;

    char *variable;
    char **list;
    struct ast_node *body;
};

struct ast_node_if
{
    struct ast_node node_base;

    struct ast_node *condition;
    struct ast_node *if_body;
    struct ast_node *else_body;
};

struct ast_node_while
{
    struct ast_node node_base;

    struct ast_node *condition;
    struct ast_node *loop;
};

struct ast_node_until
{
    struct ast_node node_base;

    struct ast_node *condition;
    struct ast_node *loop;
};

struct ast_node_command
{
    struct ast_node node_base;

    enum
    {
        simp_cmd,
        if_cmd,
        while_cmd,
        until_cmd,
        for_cmd,
    } type;
    union
    {
        struct ast_node_if *if_node;
        struct ast_node_while *while_node;
        struct ast_node_until *until_node;
        struct ast_node_simple_command *simp_node;
        struct ast_node_for *for_node;
    } value;
};

struct ast_node_pipe
{
    struct ast_node node_base;

    int is_neg;
    struct ast_node_command **commands;
};

struct ast_node_and_or
{
    struct ast_node node_base;

    enum op *ops; /* Array of n operands */
    size_t nb_ops;

    struct ast_node_pipe **pipes; /* Array of n + 1 operands */
};

struct ast_node_command_list
{
    struct ast_node node_base;

    struct ast_node_and_or **and_ors;
};

#endif /* ! AST_H */
