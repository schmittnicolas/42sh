#include "lexer_utils.h"
#define STX 2

int is_word(char c)
{
    static char special_char[] = "!|&;<>()$`\"' \t\n\0";
    for (size_t i = 0; special_char[i] != '\0'; i++)
    {
        if (c == special_char[i] || c == EOF)
            return 0;
    }

    return 1;
}

struct lexer *init_lexer(FILE *stream)
{
    if (stream == NULL)
        return NULL;
    struct lexer *lexer = malloc(sizeof(struct lexer));

    if (lexer == NULL)
        return NULL;

    lexer->stream = stream;
    lexer->cur_char = fgetc(stream);
    lexer->cur_pos = ftell(lexer->stream);

    lexer->token = NULL;

    return lexer;
}

void lexer_advance(struct lexer *lexer)
{
    if (lexer == NULL)
        return;

    if (lexer->cur_char != EOF)
    {
        lexer->cur_char = fgetc(lexer->stream);
        lexer->cur_pos = ftell(lexer->stream);
    }
}

void lexer_skip_whitespace(struct lexer *lexer)
{
    if (lexer == NULL)
        return;

    while (lexer->cur_char == ' ' || lexer->cur_char == '\t')
    {
        lexer_advance(lexer);
    }
}

void lexer_skip_comment(struct lexer *lexer)
{
    if (lexer == NULL)
        return;

    while (lexer->cur_char != EOF && lexer->cur_char != '\n')
    {
        lexer_advance(lexer);
    }
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
    else if (strcmp(value, "while") == 0)
        return WHILE;
    else if (strcmp(value, "do") == 0)
        return DO;
    else if (strcmp(value, "done") == 0)
        return DONE;
    else if (strcmp(value, "until") == 0)
        return UNTIL;
    else if (strcmp(value, "for") == 0)
        return FOR;
    else if (strcmp(value, "in") == 0)
        return IN;
    else
        return WORD;
}

struct token *get_word(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    int capacity = 16;
    int size = 0;

    char *value = malloc(capacity * sizeof(char));

    while (lexer->cur_char != '\0' && is_word(lexer->cur_char))
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            value = realloc(value, capacity * sizeof(char));
            if (value == NULL)
            {
                return NULL;
            }
        }

        value[size] = lexer->cur_char;
        size++;

        lexer_advance(lexer);
    }

    value[size] = '\0';
    enum token_type type = get_type(value);
    return init_token(type, value);
}

struct token *get_single_quoted_string(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    // Skip the firste quote
    lexer_advance(lexer);

    int capacity = 16;
    int size = 0;

    char *value = malloc(capacity * sizeof(char));

    while (lexer->cur_char != '\'')
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            value = realloc(value, capacity * sizeof(char));
            if (value == NULL)
            {
                return NULL;
            }
        }

        value[size] = lexer->cur_char;
        size++;

        lexer_advance(lexer);
    }

    lexer_advance(lexer);
    value[size] = '\0';

    enum token_type type = get_type(value);
    return init_token(type, value);
}

struct token *get_double_quoted_string(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;

    /* Skip the first double quote */
    lexer_advance(lexer);

    int capacity = 16;
    int size = 0;

    char *value = malloc(sizeof(char) * capacity);

    while (lexer->cur_char != '\"')
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            value = realloc(value, capacity * sizeof(char));
            if (value == NULL)
            {
                return NULL;
            }
        }

        if (lexer->cur_char == '\\')
        {
            lexer_advance(lexer);

            if (lexer->cur_char == '$' || lexer->cur_char == '`'
                || lexer->cur_char == '\\' || lexer->cur_char == '\n')
            {
                value[size] = lexer->cur_char;
                size++;
            }
            else
            {
                value[size] = '\\';
                size++;
                value[size] = lexer->cur_char;
                size++;
            }
        }
        else
        {
            value[size] = lexer->cur_char;
            size++;
        }

        lexer_advance(lexer);
    }

    lexer_advance(lexer);
    value[size] = '\0';
    enum token_type type = get_type(value);
    return init_token(type, value);
}
