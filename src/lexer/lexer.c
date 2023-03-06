#include "lexer.h"

struct token *get_redirection(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    int capacity = 3;
    int size = 0;

    char *value = malloc(capacity * sizeof(char));

    char c = lexer->cur_char;
    if (c == '<' || c == '>')
    {
        value[size] = lexer->cur_char;
        size++;

        lexer_advance(lexer);
    }

    c = lexer->cur_char;
    if (c == '<' || c == '>' || c == '&' || c == '|')
    {
        value[size] = lexer->cur_char;
        size++;

        lexer_advance(lexer);
    }

    value[size] = '\0';
    return init_token(REDIRECTION, value);
}

struct token *get_operator(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    int capacity = 3;
    int size = 0;

    char *value = malloc(capacity * sizeof(char));

    char c = lexer->cur_char;
    if (c == '|')
    {
        value[size] = lexer->cur_char;
        size++;

        lexer_advance(lexer);

        c = lexer->cur_char;
        if (c == '|')
        {
            value[size] = lexer->cur_char;
            size++;
            value[size] = '\0';
            lexer_advance(lexer);
            return init_token(OR, value);
        }
        else
        {
            value[size] = '\0';
            lexer_advance(lexer);
            return init_token(PIPE, value);
        }
    }
    else if (lexer->cur_char == '&')
    {
        value[size] = lexer->cur_char;
        size++;
        value[size] = '\0';
        lexer_advance(lexer);
        if (lexer->cur_char == '&')
        {
            value[size] = lexer->cur_char;
            size++;
            value[size] = '\0';
            lexer_advance(lexer);
            return init_token(AND, value);
        }
        else
        {
            free(value);
            return NULL;
        }
    }

    free(value);
    return NULL;
}

struct token *get_next_token(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    while (lexer->cur_char != EOF)
    {
        struct token *token = NULL;

        if (lexer->cur_char == ' ' || lexer->cur_char == '\t')
        {
            lexer_skip_whitespace(lexer);
        }

        if (lexer->cur_char == '#')
        {
            lexer_skip_comment(lexer);
        }

        if (is_word(lexer->cur_char))
        {
            token = get_word(lexer);
            return token;
        }

        if (lexer->cur_char == '\'')
        {
            token = get_single_quoted_string(lexer);
            return token;
        }

        if (lexer->cur_char == '\"')
        {
            token = get_double_quoted_string(lexer);
            return token;
        }

        if (lexer->cur_char == '>' || lexer->cur_char == '<')
        {
            token = get_redirection(lexer);
            return token;
        }

        if (lexer->cur_char == '|' || lexer->cur_char == '&')
        {
            token = get_operator(lexer);
            return token;
        }
        switch (lexer->cur_char)
        {
        case ';':
            token = init_token(SEMICOLON, NULL);
            lexer_advance(lexer);
            return token;
        case '\n':
            token = init_token(NEWLINE, NULL);
            lexer_advance(lexer);
            return token;
        case '!':
            token = init_token(NEG, NULL);
            lexer_advance(lexer);
            return token;
        default:
            break;
        }
    }

    return NULL;
}

enum token_type get_next_token_type(struct lexer *lexer)
{
    if (lexer == NULL)
        return ERR;

    long prev_pos = ftell(lexer->stream);

    struct token *token = get_next_token(lexer);

    if (token == NULL)
    {
        fseek(lexer->stream, prev_pos, SEEK_SET);
        lexer->cur_pos = prev_pos;
        return EOI;
    }

    fseek(lexer->stream, prev_pos, SEEK_SET);

    lexer->cur_pos = prev_pos;

    enum token_type type = token->type;
    free_token(token);

    return type;
}

struct token *get_token(struct lexer *lexer)
{
    if (is_empty(lexer))
        return NULL;

    if (lexer->token == NULL)
    {
        /* first call to get_next_token */

        lexer->token = get_next_token(lexer);
    }

    return lexer->token;
}

void eat_token(struct lexer *lexer)
{
    struct token *token = lexer->token;
    free_token(token);
    lexer->token = get_next_token(lexer);
}

int is_empty(struct lexer *lexer)
{
    return (lexer == NULL || (lexer->token == NULL && lexer->cur_char == EOF));
}

void free_lexer(struct lexer *lexer)
{
    if (lexer == NULL)
        return;

    free(lexer->token);
    free(lexer);
}
