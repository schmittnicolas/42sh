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
    free(token->value);
    free(token);
}
