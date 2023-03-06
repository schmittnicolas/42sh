#include "token.h"

struct token *init_token(enum token_type type, char *value)
{
    struct token *token = malloc(sizeof(struct token));
    if (token == NULL)
        return NULL;

    token->type = type;
    token->value = value;

    return token;
}

void free_token(struct token *token)
{
    if (token == NULL)
        return;

    free(token->value);
    free(token);
}

int is_word_token(enum token_type type)
{
    return (type == WORD || type == IF || type == THEN || type == ELSE
            || type == THEN || type == FI || type == ELIF || type == WHILE
            || type == DO || type == DONE || type == UNTIL || type == NEG);
}
