#ifndef PARSER_2_BIS
#define PARSER_2_BIS

#include "parser2.h"

/* If Command */
struct ast_node_if *parse_if_command(struct lexer *lexer);
void print_ast_node_if(struct ast_node_if *node);
void free_ast_node_if(struct ast_node_if *node);

/* For */
struct ast_node_for *parse_for(struct lexer *lexer);
void free_ast_node_for(struct ast_node_for *node);
void print_ast_node_for(struct ast_node_for *node);

/* Command */
struct ast_node_command *parse_command(struct lexer *lexer);
void free_ast_node_command(struct ast_node_command *node);
void print_ast_node_command(struct ast_node_command *node);

#endif /* ! PARSER_2_BIS */
