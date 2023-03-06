#ifndef PARSER2_H
#define PARSER2_H

#include "parser1.h"

/* Until */
struct ast_node_until *parse_until(struct lexer *lexer);
void free_ast_node_until(struct ast_node_until *node);
void print_ast_node_until(struct ast_node_until *node);

/* Pipe */
struct ast_node_pipe *parse_pipe(struct lexer *lexer);
void free_ast_node_pipe(struct ast_node_pipe *node);
void print_ast_node_pipe(struct ast_node_pipe *node);

/* Command List */
struct ast_node_command_list *parse_command_list(struct lexer *lexer);
void print_ast_node_command_list(struct ast_node_command_list *node);
void free_ast_node_command_list(struct ast_node_command_list *node);

/* Compound List */
struct ast_node_command_list *parse_compound_list(struct lexer *lexer);

#endif /* ! PARSER2_H */
