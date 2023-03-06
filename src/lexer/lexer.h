#ifndef LEXER_H
#define LEXER_H

#include "lexer_utils.h"

struct token *get_token(struct lexer *lexer);

/* A call to this function is necessary to get the next token */
void eat_token(struct lexer *lexer);

int is_empty(struct lexer *lexer);

void free_lexer(struct lexer *lexer);

#endif /* LEXER_H */
