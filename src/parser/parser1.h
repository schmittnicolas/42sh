#ifndef PARSER1_H
#define PARSER1_H

#include "ast.h"

/* Base Node */
struct ast_node *parse(struct lexer *lexer);
void print_ast_node(struct ast_node *node);
void free_ast_node(struct ast_node *node);

/* Simple Command */
struct ast_node_simple_command *parse_simple_command(struct lexer *lexer);
void print_ast_node_simple_command(struct ast_node_simple_command *node);
void free_ast_node_simple_command(struct ast_node_simple_command *node);

/* While Command */
struct ast_node_while *parse_while(struct lexer *lexer);
void print_ast_node_while(struct ast_node_while *node);
void free_ast_node_while(struct ast_node_while *node);

#endif /* ! PARSER_H */
