#ifndef SK_TOK_H
#define SK_TOK_H

#include <stdint.h>
#include <stddef.h>

enum sk_token_type
{
    // One character tokens
    SK_TOK_LPAREN,
    SK_TOK_RPAREN,
    SK_TOK_LBRACE,
    SK_TOK_RBRACE,
    SK_TOK_PLUS,
    SK_TOK_MINUS,
    SK_TOK_STAR,
    SK_TOK_SLASH,
    SK_TOK_SEMICOLON,
    SK_TOK_COMMA,
    SK_TOK_DOT,

    // One or two character tokens.
    SK_TOK_BANG,
    SK_TOK_BANG_EQUAL,
    SK_TOK_EQUAL,
    SK_TOK_EQUAL_EQUAL,
    SK_TOK_GREATER,
    SK_TOK_GREATER_EQUAL,
    SK_TOK_LESS,
    SK_TOK_LESS_EQUAL,

    // Literals
    SK_TOK_IDENTIFIER,
    SK_TOK_STRING_LITERAL,
    SK_TOK_FLOATING_POINT_LITERAL,
    SK_TOK_INTEGER_LITERAL,

    // Control flow
    SK_TOK_IF,
    SK_TOK_ELSE,
    SK_TOK_RETURN,
    SK_TOK_WHILE,
    SK_TOK_FOR,

    SK_TOK_TRUE,
    SK_TOK_FALSE,

    // Types
    SK_TOK_INT,
    SK_TOK_UINT,
    SK_TOK_FLOAT,
    SK_TOK_DOUBLE,
    SK_TOK_BOOL,
    SK_TOK_VOID,

    SK_TOK_STRUCT,
    SK_TOK_ENUM,

    SK_TOK_ILLEGAL,
    SK_TOK_EOF
};

struct sk_token
{
    enum sk_token_type type;
    const char* start;
    size_t length;

    size_t line;
    size_t column;
};

void sk_tok_init(const char* source);
struct sk_token sk_tok_next(void);

#endif // SK_TOK_H
