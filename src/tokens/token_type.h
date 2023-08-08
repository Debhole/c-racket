#pragma once

#include <stdio.h>

typedef enum token_type_t {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_BACKTICK,

    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_BOOLEAN,
    TOKEN_SYMBOL
} token_type_t;

inline void print_token(token_type_t token) {
    switch (token) {
        case TOKEN_LEFT_PAREN:
            printf("Token: (\n");
            break;
        case TOKEN_RIGHT_PAREN:
            printf("Token: )\n");
            break;
        case TOKEN_BACKTICK:
            printf("Token: `\n");
            break;
        case TOKEN_STRING:
            printf("Token: String Literal");
            break;
        case TOKEN_NUMBER:
            printf("Token: Number Literal");
            break;
        case TOKEN_BOOLEAN:
            printf("Token: Boolean Literal");
            break;
        case TOKEN_SYMBOL:
            printf("Token: Symbol Literal");
            break;
    }
}