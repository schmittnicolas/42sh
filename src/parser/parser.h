#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>
#include "../lexer/lexer.h"

enum node_type
{
    SIMPLE_COMMAND,
    ELEMENT,
};

struct ast_node
{
    enum node_type e_type;
};

struct ast_node_simple_command
{
    struct ast_node *node_base;

    /* simple command field */
    char **words;
};


/* Simple Command */
struct ast_node_simple_command *parse_simple_command(struct lexer *lexer);
void print_ast_node_simple_command(struct ast_node_simple_command *node);
void free_ast_node_simple_command(struct ast_node_simple_command *node);



#endif /* PARSER_H */
