#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

const char* token_names[] =
{
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
    "SEMICOLON",
    "COMMA",
    "DOT",
    "BANG",
    "BANG_EQUAL",
    "EQUAL",
    "EQUAL_EQUAL",
    "GREATER",
    "GREATER_EQUAL",
    "LESS",
    "LESS_EQUAL",
    "IDENTIFIER",
    "STRING_LITERAL",
    "FLOATING_POINT_LITERAL",
    "INTEGER_LITERAL",
    "IF",
    "ELSE",
    "RETURN",
    "WHILE",
    "FOR",
    "TRUE",
    "FALSE",
    "INT",
    "UINT",
    "FLOAT",
    "DOUBLE",
    "BOOL",
    "VOID",
    "STRUCT",
    "ENUM",
    "ILLEGAL",
    "EOF"
};

char* read_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size)
    {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);

    return buffer;
}

int main(void)
{
    const char* source = read_file("test.sk");
    sk_tok_init(source);

    struct sk_token token = sk_tok_next();
    while(token.type != SK_TOK_EOF)
    {
        size_t size = token.length;
    
        char identifier[size + 1];
        strncpy(identifier, token.start, size);
        identifier[size] = '\0';

        printf("Type: %s, Value: %s\n", token_names[token.type], identifier);
        token = sk_tok_next();
    }
}
