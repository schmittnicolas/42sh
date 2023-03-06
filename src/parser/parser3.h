#ifndef PARSER_H
#define PARSER_H

#include "parser2bis.h"

/* And Or */
struct ast_node_and_or *parse_and_or(struct lexer *lexer);
void free_ast_node_and_or(struct ast_node_and_or *node);
void print_ast_node_and_or(struct ast_node_and_or *node);

#endif /* PARSER_H */
