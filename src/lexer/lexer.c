#include "lexer.h"

struct lexer *init_lexer(char *input)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));

    if (lexer == NULL)
        return NULL;

    lexer->source = input;
    lexer->cur_char = input[0];
    lexer->cur_pos = 0;
    lexer->size = strlen(input);

    return lexer;
}

void lexer_advance(struct lexer *lexer)
{
    if (lexer->cur_char != '\0' && lexer->cur_pos < lexer->size)
    {
        lexer->cur_pos += 1;
        lexer->cur_char = lexer->source[lexer->cur_pos];
    }
}

void lexer_skip_whitespace(struct lexer *lexer)
{
    while (lexer->cur_char == ' ' || lexer->cur_char == '\t')
    {
        lexer_advance(lexer);
    }
}

char *string_from_char(struct lexer *lexer)
{
    char *str = malloc(sizeof(char) * 2);
    if (str == NULL)
        return NULL;

    str[0] = lexer->cur_char;
    str[1] = '\0';

    return str;
}

enum token_type get_type(char *value)
{
    if (strcmp(value, "if") == 0)
        return IF;
    else if (strcmp(value, "then") == 0)
        return THEN;
    else if (strcmp(value, "elif") == 0)
        return ELIF;
    else if (strcmp(value, "else") == 0)
        return ELSE;
    else if (strcmp(value, "fi") == 0)
        return FI;
    else if (strcmp(value, ";") == 0)
        return SEMICOLON;
    else if (strcmp(value, "'") == 0)
        return SINGLE_QUOTE;
    else
        return WORD;
}

struct token *get_word(struct lexer *lexer)
{
    char *value = malloc(sizeof(char));
    value[0] = '\0';

    while(isalnum(lexer->cur_char))
    {
        char *str = string_from_char(lexer);
        value = realloc(value, (strlen(value) + strlen(str) + 1) * sizeof(char));
        strcat(value, str);

        lexer_advance(lexer);
    }

    enum token_type type = get_type(value);

    return init_token(type, value);
}

struct token *get_single_quoted_string(struct lexer *lexer)
{
    lexer_advance(lexer);

    char *value = malloc(sizeof(char));
    value[0] = '\0';

    while(lexer->cur_char != '\'')
    {
        char *str = string_from_char(lexer);
        value = realloc(value, (strlen(value) + strlen(str) + 1) * sizeof(char));
        strcat(value, str);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    enum token_type type = get_type(value);

    return init_token(type, value);
}

struct token *get_next_token(struct lexer *lexer)
{
    while (lexer->cur_char != '\0' && lexer->cur_pos < lexer->size)
    {
        if (lexer->cur_char == ' ' || lexer->cur_char == '\t')
        {
            lexer_skip_whitespace(lexer);
        }

        if (isalnum(lexer->cur_char))
        {
            return get_word(lexer);
        }

        if (lexer->cur_char == '\'')
        {
            return get_single_quoted_string(lexer);
        }
        
        struct token *token = NULL;
        switch(lexer->cur_char)
        {
            case ';':
                token = init_token(SEMICOLON, string_from_char(lexer));
                lexer_advance(lexer);
                return token;
            case '\n':
                token = init_token(NEWLINE, string_from_char(lexer));
                lexer_advance(lexer);
                return token;
            default:
                break;
        }

        return init_token(NEWLINE, string_from_char(lexer));
    }

    return NULL;
}

void free_lexer(struct lexer *lexer)
{
    free(lexer);
}


/*
int main(void)
{
    char *input = "echo this as an input; else then if 'h hsh shsh'";
    struct lexer *lexer = init_lexer(input);

    struct token *token = get_next_token(lexer);

    while (token != NULL)
    {
        printf("token: (%d, %s)\n", token->type, token->value);
        token = get_next_token(lexer);

    }

    free_lexer(lexer);

    return 0;
}*/
