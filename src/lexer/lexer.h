#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

struct lexer
{
    char cur_char;
    size_t cur_pos;
    char *source;
    size_t size;
};

struct lexer *init_lexer(char *input);
void lexer_advance(struct lexer *lexer);
void lexer_skip_whitespace(struct lexer *lexer);

struct token *get_word(struct lexer *lexer);
struct token *get_next_token(struct lexer *lexer);
struct token *get_single_quote(struct lexer *lexer);

void free_lexer(struct lexer *lexer);

#endif /* LEXER_H */
