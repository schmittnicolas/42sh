#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../io/stream.h"
#include "token.h"

struct lexer
{
    char cur_char;
    long cur_pos;
    FILE *stream;

    struct token *token;
};

int is_word(char c);
struct lexer *init_lexer(FILE *stream);
void lexer_advance(struct lexer *lexer);
void lexer_skip_whitespace(struct lexer *lexer);
void lexer_skip_comment(struct lexer *lexer);
enum token_type get_type(char *value);
struct token *get_word(struct lexer *lexer);
struct token *get_single_quoted_string(struct lexer *lexer);
struct token *get_double_quoted_string(struct lexer *lexer);

#endif /* LEXER_UTILS_H */
