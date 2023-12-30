#include "tokenizer.h"

#include <stdbool.h>
#include <string.h>

struct keyword_entry
{
    const char* keyword;
    enum sk_token_type token;
};

static struct keyword_entry keyword_map[] =
{
    {"if", SK_TOK_IF},
    {"else", SK_TOK_ELSE},
    {"return", SK_TOK_RETURN},
    {"while", SK_TOK_WHILE},
    {"for", SK_TOK_FOR},
    {"true", SK_TOK_TRUE},
    {"false", SK_TOK_FALSE},
    {"int", SK_TOK_INT},
    {"uint", SK_TOK_UINT},
    {"float", SK_TOK_FLOAT},
    {"double", SK_TOK_DOUBLE},
    {"bool", SK_TOK_BOOL},
    {"void", SK_TOK_VOID},
    {"struct", SK_TOK_STRUCT},
    {"enum", SK_TOK_ENUM},
};

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool is_alphanumeric(char c)
{
    return is_digit(c) || is_alpha(c);
}

static const char* start;
static const char* current;
static size_t line;
static size_t column;

static bool is_at_end(void)
{
    return *current == '\0';
}

static char advance(void)
{
    column++;
    return *(current++);
}

static char peek(void)
{
    return *current;
}

static char peek_next(void) 
{
    return *(current + 1);
}

static bool match(char expected)
{
    if(*current != expected)
    {
        return false;
    }
    else
    {
        current++;
        return true;
    }
}

static void skip_whitespace(void)
{
    for(;;)
    {
        char c = peek();

        switch(c)
        {
            // Skip whitespace (spaces, carriage returns and tabs)
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            
            // Newlines can be skipped after resetting the current column and incrementing the current line.
            case '\n':
                column = 0;
                line++;
                advance();
                break;
            
            // Comments are skipped by advancing to the end of the line.
            case '#':
                while(peek() != '\n')
                {
                    advance();
                }
                break;
            
            default:
                return;
        }
    }
}

static struct sk_token tok_create(enum sk_token_type type)
{
    struct sk_token token =
    {
        .type = type,
        .start = start,
        .length = current - start,
        .line = line,
        .column = column
    };

    return token;
}

static struct sk_token tok_error(const char* message)
{
    struct sk_token token =
    {
        .type = SK_TOK_ILLEGAL,
        .start = message,
        .length = strlen(message),
        .line = line,
        .column = column
    };

    return token;
}

static struct sk_token consume_string(void)
{
    while(peek() != '"' && !is_at_end())
    {
        if(peek() == '\n') 
        {
            column = 0;
            line++;
        }

        advance();
    }

    // Reaching the end of the file before finding the closing quotation mark means the string is unterminated.
    if(is_at_end())
    {
        return tok_error("Unterminated string");
    }

    // Consume the closing closing quotation mark.
    advance();

    return tok_create(SK_TOK_STRING_LITERAL);
}

static struct sk_token consume_number(void)
{
    bool was_float = false;

    while(is_digit(peek()))
    {
        advance();
    }

    // A floating point literal cannot end on a period. There should still be a second digit after the period.
    if(peek() == '.' && is_digit(peek_next()))
    {
        advance();

        while(is_digit(peek()))
        {
            advance();
        }

        was_float = true;
    }

    if(was_float)
    {
        return tok_create(SK_TOK_FLOATING_POINT_LITERAL);
    }
    else
    {
        return tok_create(SK_TOK_INTEGER_LITERAL);
    }
}

static struct sk_token consume_identifier(void)
{
    while(is_alphanumeric(peek()))
    {
        advance();
    }

    size_t size = current - start;

    char identifier[size + 1];
    strncpy(identifier, start, size);
    identifier[size] = '\0';

    for(size_t i = 0; i < (sizeof(keyword_map) / sizeof(struct keyword_entry)); i++)
    {
        if(strcmp(identifier, keyword_map[i].keyword) == 0)
        {
            return tok_create(keyword_map[i].token);
        }
    }

    return tok_create(SK_TOK_IDENTIFIER);
}

void sk_tok_init(const char* source)
{
    start = source;
    current = source;
    line = 1;
    column = 1;
}

struct sk_token sk_tok_next(void)
{
    skip_whitespace();

    start = current;

    if(is_at_end())
    {
        return tok_create(SK_TOK_EOF);
    }

    char c = advance();

    switch(c)
    {
        case '(': return tok_create(SK_TOK_LPAREN);
        case ')': return tok_create(SK_TOK_RPAREN);
        case '{': return tok_create(SK_TOK_LBRACE);
        case '}': return tok_create(SK_TOK_RBRACE);
        case '+': return tok_create(SK_TOK_PLUS);
        case '-': return tok_create(SK_TOK_MINUS);
        case '*': return tok_create(SK_TOK_STAR);
        case '/': return tok_create(SK_TOK_SLASH);
        case ';': return tok_create(SK_TOK_SEMICOLON);
        case ',': return tok_create(SK_TOK_COMMA);
        case '.': return tok_create(SK_TOK_DOT);
        
        case '!':
            if(match('='))
            {
                return tok_create(SK_TOK_BANG_EQUAL);
            }
            else
            {
                return tok_create(SK_TOK_BANG);
            }

        case '=':
            if(match('='))
            {
                return tok_create(SK_TOK_EQUAL_EQUAL);
            }
            else
            {
                return tok_create(SK_TOK_EQUAL);
            }
        
        case '>':
            if(match('='))
            {
                return tok_create(SK_TOK_GREATER_EQUAL);
            }
            else
            {
                return tok_create(SK_TOK_GREATER);
            }

        case '<':
            if(match('='))
            {
                return tok_create(SK_TOK_LESS_EQUAL);
            }
            else
            {
                return tok_create(SK_TOK_LESS);
            }

        case '"':
            return consume_string();
            break;

        default:
            if(is_digit(c))
            {
                return consume_number();
            }
            else if(is_alpha(c))
            {
                return consume_identifier();
            }
            else
            {
                return tok_error("Unexpected character");
            }
            break;
    }
}
