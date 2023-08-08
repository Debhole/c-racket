#pragma once

#include <string.h>

typedef enum token_type_t {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,

    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_BOOLEAN,
    TOKEN_SYMBOL,

    TOKEN_EOF,
    TOKEN_ERROR,
} token_type_t;

inline const char *token_type_to_string(token_type_t token) {
    switch (token) {
        case TOKEN_LEFT_PAREN:
            return "Left Parenthesis - '('";
        case TOKEN_RIGHT_PAREN:
            return "Right Parenthesis - ')'";
        case TOKEN_STRING:
            return "String Literal";
        case TOKEN_NUMBER:
            return "Number Literal";
        case TOKEN_BOOLEAN:
            return "Boolean Literal";
        case TOKEN_SYMBOL:
            return "Symbol";
        case TOKEN_EOF:
            return "End of File";
        case TOKEN_ERROR:
            return "Error";
    }
}