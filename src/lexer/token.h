#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdlib.h>

enum token_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    WHILE,
    DO,
    DONE,
    UNTIL,
    FOR,
    IN,
    SEMICOLON,
    NEWLINE,
    SINGLE_QUOTE,
    WORD,
    PIPE,
    AND,
    OR,
    NEG,
    REDIRECTION,
    ERR,
    EOI,
};

struct token
{
    enum token_type type;
    char *value;
};

struct token *init_token(enum token_type type, char *value);
void free_token(struct token *token);
int is_word_token(enum token_type type);

#endif /* TOKEN_H */
