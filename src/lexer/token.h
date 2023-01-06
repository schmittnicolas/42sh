#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stddef.h>

enum token_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMICOLON,
    NEWLINE,
    SINGLE_QUOTE,
    WORD
};

struct token
{
    enum token_type type;
    char *value;
};

struct token *init_token(enum token_type type, char *value);
void free_token(struct token *token);

#endif /* TOKEN_H */
